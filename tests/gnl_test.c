/*
** gnl_test.c : NOT part of the submission (mandatory part only).
**
** For every test it builds some content, puts it behind a file descriptor
** (regular file, pipe, ...), then calls get_next_line() in a loop and compares
** every line with a reference splitter written in this file:
**   - each line must end with '\n' (except the last one if the content has none)
**   - after the last line, get_next_line() must return NULL, again and again
**
** Run with the helper script:     bash tests/run_tests.sh
** Or by hand:
**   cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -I. get_next_line.c \
**      get_next_line_utils.c tests/gnl_test.c -o gnl_test && ./gnl_test
** Read from the keyboard or a pipe:   ./gnl_test stdin      (Ctrl-D = end)
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include "get_next_line.h"

static int	g_ok;
static int	g_ko;
static int	g_skip;

static void	preview(const char *s, size_t n, char *out, size_t outsz)
{
	size_t	i;
	size_t	o;

	i = 0;
	o = 0;
	while (i < n && i < 30 && o + 3 < outsz)
	{
		if (s[i] == '\n')
		{
			out[o++] = '\\';
			out[o++] = 'n';
		}
		else
			out[o++] = s[i];
		i++;
	}
	if (i < n && o + 4 < outsz)
	{
		out[o++] = '.';
		out[o++] = '.';
		out[o++] = '.';
	}
	out[o] = '\0';
}

static void	report(const char *label, int ok, const char *err)
{
	if (ok)
	{
		g_ok++;
		printf("[OK] %s\n", label);
		return ;
	}
	g_ko++;
	printf("[KO] %s\n     -> %s\n", label, err);
}

/* reads everything from fd and compares with the reference split of content */
static int	check_fd(int fd, const char *content, char *err, size_t errsz)
{
	size_t	pos;
	size_t	total;
	size_t	explen;
	int		lineno;
	char	*line;
	char	*nl;
	char	pe[80];
	char	pg[80];
	int		i;

	pos = 0;
	total = strlen(content);
	lineno = 0;
	while (pos < total)
	{
		nl = strchr(content + pos, '\n');
		explen = nl ? (size_t)(nl - (content + pos)) + 1 : total - pos;
		lineno++;
		line = get_next_line(fd);
		if (!line)
		{
			preview(content + pos, explen, pe, sizeof(pe));
			snprintf(err, errsz, "call #%d returned NULL, expected \"%s\" (%zu chars)",
				lineno, pe, explen);
			return (0);
		}
		if (strlen(line) != explen || memcmp(line, content + pos, explen) != 0)
		{
			preview(content + pos, explen, pe, sizeof(pe));
			preview(line, strlen(line), pg, sizeof(pg));
			snprintf(err, errsz, "call #%d: expected \"%s\" (%zu chars), got \"%s\" (%zu chars)",
				lineno, pe, explen, pg, strlen(line));
			free(line);
			return (0);
		}
		free(line);
		pos += explen;
	}
	i = 0;
	while (i < 3)
	{
		line = get_next_line(fd);
		if (line)
		{
			preview(line, strlen(line), pg, sizeof(pg));
			snprintf(err, errsz, "after the last line, call #%d returned \"%s\" instead of NULL",
				i + 1, pg);
			free(line);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	write_all(int fd, const char *s, size_t n)
{
	ssize_t	w;

	while (n > 0)
	{
		w = write(fd, s, n);
		if (w <= 0)
			return ;
		s += w;
		n -= (size_t)w;
	}
}

static void	test_file(const char *label, const char *content)
{
	char	path[] = "/tmp/gnl_test_XXXXXX";
	char	err[300];
	int		fd;
	int		ok;

	fd = mkstemp(path);
	if (fd < 0)
	{
		report(label, 0, "could not create the temporary file");
		return ;
	}
	write_all(fd, content, strlen(content));
	lseek(fd, 0, SEEK_SET);
	err[0] = '\0';
	ok = check_fd(fd, content, err, sizeof(err));
	close(fd);
	unlink(path);
	report(label, ok, err);
}

static void	test_pipe(const char *label, const char *content)
{
	int		p[2];
	char	err[300];
	int		ok;

	if (strlen(content) > 60000 || pipe(p) < 0)
		return ;
	write_all(p[1], content, strlen(content));
	close(p[1]);
	err[0] = '\0';
	ok = check_fd(p[0], content, err, sizeof(err));
	close(p[0]);
	report(label, ok, err);
}

/* the writer sends small pieces with pauses: read() returns less than BUFFER_SIZE */
static void	test_slow_pipe(void)
{
	int		p[2];
	pid_t	pid;
	char	err[300];
	int		ok;

	if (pipe(p) < 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		write_all(p[1], "ab", 2);
		usleep(150000);
		write_all(p[1], "c\nd", 3);
		usleep(150000);
		write_all(p[1], "e\n\nf", 4);
		close(p[1]);
		_exit(0);
	}
	close(p[1]);
	err[0] = '\0';
	ok = check_fd(p[0], "abc\nde\n\nf", err, sizeof(err));
	close(p[0]);
	waitpid(pid, NULL, 0);
	report("pipe: data arrives in small pieces with pauses", ok, err);
}

static double	now(void)
{
	struct timespec	t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return ((double)t.tv_sec + (double)t.tv_nsec / 1e9);
}

/* two lines arrive in ONE read(); asking for the 2nd line must not read again */
static void	test_no_extra_read(void)
{
	int		p[2];
	pid_t	pid;
	char	*l1;
	char	*l2;
	char	*l3;
	double	t0;
	double	dt;

	if (BUFFER_SIZE < 8)
	{
		g_skip++;
		printf("[--] no unnecessary read() (needs BUFFER_SIZE >= 8, skipped)\n");
		return ;
	}
	if (pipe(p) < 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		write_all(p[1], "one\ntwo\n", 8);
		sleep(1);
		write_all(p[1], "three\n", 6);
		close(p[1]);
		_exit(0);
	}
	close(p[1]);
	l1 = get_next_line(p[0]);
	t0 = now();
	l2 = get_next_line(p[0]);
	dt = now() - t0;
	l3 = get_next_line(p[0]);
	free(l1);
	free(l2);
	free(l3);
	close(p[0]);
	waitpid(pid, NULL, 0);
	if (dt > 0.5)
	{
		char	msg[200];

		snprintf(msg, sizeof(msg),
			"2nd call took %.2fs: it called read() although the line was already in the stash", dt);
		report("no unnecessary read() when a full line is already stored", 0, msg);
	}
	else
		report("no unnecessary read() when a full line is already stored", 1, "");
}

static void	test_invalid_fds(void)
{
	int		fd;
	char	*l;

	l = get_next_line(-1);
	report("fd = -1 returns NULL", l == NULL, "did not return NULL");
	free(l);
	l = get_next_line(1000);
	report("fd = 1000 (never opened) returns NULL", l == NULL, "did not return NULL");
	free(l);
	fd = open("/dev/null", O_RDONLY);
	l = get_next_line(fd);
	report("/dev/null returns NULL", l == NULL, "did not return NULL");
	free(l);
	close(fd);
	l = get_next_line(fd);
	report("closed fd returns NULL", l == NULL, "did not return NULL");
	free(l);
	fd = open(".", O_RDONLY);
	l = get_next_line(fd);
	report("directory (read() fails) returns NULL", l == NULL, "did not return NULL");
	free(l);
	l = get_next_line(fd);
	report("directory, second call, returns NULL", l == NULL, "did not return NULL");
	free(l);
	close(fd);
}

static char	*repeat_char(size_t n, char c, int add_nl)
{
	char	*s;

	s = malloc(n + 2);
	if (!s)
		return (NULL);
	memset(s, c, n);
	if (add_nl)
		s[n++] = '\n';
	s[n] = '\0';
	return (s);
}

static void	test_boundaries(void)
{
	size_t	b;
	size_t	lens[5];
	char	label[120];
	char	*s;
	int		i;
	int		nl;

	b = (size_t)BUFFER_SIZE;
	if (b > 20000000)
		return ;
	lens[0] = b - 1;
	lens[1] = b;
	lens[2] = b + 1;
	lens[3] = 2 * b;
	lens[4] = 2 * b + 1;
	i = 0;
	while (i < 5)
	{
		nl = 0;
		while (nl < 2)
		{
			if (lens[i] > 0)
			{
				s = repeat_char(lens[i], 'x', nl);
				snprintf(label, sizeof(label),
					"one line of %zu chars %s newline (around BUFFER_SIZE)",
					lens[i], nl ? "with" : "without");
				if (s)
					test_file(label, s);
				free(s);
			}
			nl++;
		}
		i++;
	}
	s = malloc(b * 3 + 10);
	if (s)
	{
		char	*a;
		char	*c;

		a = repeat_char(b, 'a', 1);
		c = repeat_char(b, 'c', 0);
		if (a && c)
		{
			strcpy(s, a);
			strcat(s, a);
			strcat(s, c);
			test_file("three lines of exactly BUFFER_SIZE chars (last without newline)", s);
		}
		free(a);
		free(c);
	}
	free(s);
}

static void	test_many_lines(void)
{
	char	*s;
	size_t	i;
	size_t	off;

	s = malloc(20000);
	if (!s)
		return ;
	off = 0;
	i = 0;
	while (i < 1000)
	{
		off += (size_t)snprintf(s + off, 20000 - off, "line %zu\n", i);
		i++;
	}
	test_file("1000 short lines", s);
	test_pipe("pipe: 1000 short lines", s);
	i = 0;
	off = 0;
	while (i < 60)
	{
		memset(s + off, 'a' + (int)(i % 26), i);
		off += i;
		s[off++] = '\n';
		i++;
	}
	s[off] = '\0';
	test_file("lines of length 0 to 59", s);
	free(s);
}

static void	test_content(void)
{
	char	*big;

	test_file("empty file", "");
	test_file("only one newline", "\n");
	test_file("three newlines", "\n\n\n");
	test_file("one char, no newline", "a");
	test_file("one char + newline", "a\n");
	test_file("word, no newline", "abc");
	test_file("word + newline", "abc\n");
	test_file("3 lines, no trailing newline", "line1\nline2\nline3");
	test_file("3 lines, trailing newline", "line1\nline2\nline3\n");
	test_file("starts with an empty line", "\nstarts with empty line\n");
	test_file("ends with two newlines", "ends with two newlines\n\n");
	test_file("empty lines in the middle", "a\n\nb\n\n\nc");
	test_file("spaces, tabs, symbols", "  tab\there \t\n!@#$%^&*()_+ {}[]\n\"quote\" 'q'\n");
	test_file("UTF-8 text", "h\xc3\xa9llo w\xc3\xb6rld\n\xe2\x82\xac 100\n");
	test_pipe("pipe: empty", "");
	test_pipe("pipe: one char, no newline", "a");
	test_pipe("pipe: word + newline", "abc\n");
	test_pipe("pipe: 3 lines, no trailing newline", "line1\nline2\nline3");
	big = repeat_char(10000, 'z', 1);
	if (big)
		test_file("one line of 10000 chars with newline", big);
	free(big);
	big = repeat_char(10000, 'z', 0);
	if (big)
		test_file("one line of 10000 chars without newline", big);
	free(big);
	big = repeat_char(100000, 'y', 0);
	if (big)
		test_file("one line of 100000 chars without newline", big);
	free(big);
}

static void	stdin_mode(void)
{
	char	*l;
	int		n;

	n = 0;
	while ((l = get_next_line(0)))
	{
		printf("line %d: [%s]", ++n, l);
		if (!strchr(l, '\n'))
			printf("  (no newline at the end)");
		printf("\n");
		free(l);
	}
	printf("get_next_line returned NULL (end of input)\n");
}

int	main(int argc, char **argv)
{
	if (argc > 1 && strcmp(argv[1], "stdin") == 0)
	{
		stdin_mode();
		return (0);
	}
	setvbuf(stdout, NULL, _IOLBF, 0);
	printf("===== get_next_line tests, BUFFER_SIZE = %d =====\n", BUFFER_SIZE);
	test_invalid_fds();
	test_content();
	test_file("normal file right after the error tests (state is clean)", "ok1\nok2\n");
	test_boundaries();
	test_many_lines();
	test_slow_pipe();
	test_no_extra_read();
	printf("\n===== RESULT (BUFFER_SIZE=%d): %d OK / %d KO / %d skipped =====\n",
		BUFFER_SIZE, g_ok, g_ko, g_skip);
	return (g_ko != 0);
}
