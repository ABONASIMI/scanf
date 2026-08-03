# ft_scanf

## Overview

`ft_scanf` is a simplified implementation of the standard C `scanf` function.

It reads formatted input from `stdin` and stores the converted values in variables passed by address.

This version supports only these conversions:

- `%c` — read one character
- `%d` — read one signed decimal integer
- `%s` — read one word

It is designed for the 42 Exam Rank 3 `ft_scanf` exercise.

---

## Function prototype

```c
int	ft_scanf(const char *format, ...);
```

The return value is the number of successful conversions.

Example:

```c
int		number;
char	word[100];
char	letter;

int ret = ft_scanf("%d %s %c", &number, word, &letter);
```

If all three conversions succeed:

```text
ret == 3
```

---

## Compilation

The submitted file does not need a `main()` function.

Compile it as an object file:

```bash
cc -Wall -Wextra -Werror -c ft_scanf.c
```

To test it with your own `main.c`:

```bash
cc -Wall -Wextra -Werror ft_scanf.c main.c -o test_scanf
```

Run:

```bash
./test_scanf
```

---

## Supported conversions

### `%c`

Reads exactly one character.

Unlike `%d` and `%s`, `%c` does not automatically skip spaces.

Example input:

```text
A
```

Code:

```c
char c;

ft_scanf("%c", &c);
```

Result:

```text
c == 'A'
```

---

### `%d`

Reads a signed decimal integer.

It:

- skips leading whitespace;
- accepts an optional `+` or `-` sign;
- reads consecutive decimal digits;
- stops at the first non-digit character.

Example input:

```text
-42
```

Code:

```c
int number;

ft_scanf("%d", &number);
```

Result:

```text
number == -42
```

---

### `%s`

Reads a sequence of non-whitespace characters.

It:

- skips leading whitespace;
- copies characters until a whitespace character or EOF;
- adds `'\0'` at the end of the destination string.

Example input:

```text
hello
```

Code:

```c
char word[100];

ft_scanf("%s", word);
```

Result:

```text
word == "hello"
```

The caller must provide a destination buffer large enough to hold the input.

---

## Program structure

The implementation is divided into small helper functions.

### `match_space`

```c
int	match_space(FILE *f);
```

This function skips whitespace characters from the input stream.

It:

1. reads characters with `fgetc`;
2. continues while `isspace` is true;
3. puts the first non-whitespace character back with `ungetc`;
4. returns `1` on success;
5. returns `-1` if a stream error occurs.

Whitespace includes spaces, tabs, and newline characters.

---

### `match_char`

```c
int	match_char(FILE *f, char expected);
```

This function matches one literal character from the format string.

Example format:

```c
"age=%d"
```

The characters `a`, `g`, `e`, and `=` are matched by `match_char`.

It returns:

```text
1  if the input character matches
0  if the character does not match
-1 if EOF is reached
```

If the character does not match, it is placed back into the stream with `ungetc`.

---

### `scan_char`

```c
int	scan_char(FILE *f, va_list ap);
```

This function handles the `%c` conversion.

It:

1. reads one character with `fgetc`;
2. gets the destination address with `va_arg`;
3. stores the character in the destination;
4. returns `1` on success;
5. returns `-1` on EOF.

It does not skip leading whitespace.

---

### `scan_int`

```c
int	scan_int(FILE *f, va_list ap);
```

This function handles the `%d` conversion.

It:

1. reads the first character;
2. checks for an optional `+` or `-`;
3. reads consecutive digits;
4. builds the integer using:

```c
total = total * 10 + (c - '0');
```

5. puts the first non-digit character back with `ungetc`;
6. stores the final signed value in the destination integer.

It returns:

```text
1  when a valid integer is read
0  when no digit is found
-1 when EOF is reached before reading input
```

---

### `scan_string`

```c
int	scan_string(FILE *f, va_list ap);
```

This function handles the `%s` conversion.

It:

1. reads characters until whitespace or EOF;
2. stores them in the destination buffer;
3. adds the final null terminator:

```c
dest[i] = '\0';
```

4. puts the delimiter back with `ungetc` when needed.

It returns:

```text
1  when a string is read
0  when no character is stored
-1 when EOF is reached before reading input
```

---

### `match_conv`

```c
int	match_conv(FILE *f, const char **format, va_list ap);
```

This function chooses the correct conversion function.

```c
%c -> scan_char
%d -> match_space + scan_int
%s -> match_space + scan_string
```

Unsupported conversions return `-1`.

---

### `ft_vfscanf`

```c
int	ft_vfscanf(FILE *f, const char *format, va_list ap);
```

This is the main parsing function.

It reads the format string one character at a time.

For each format character:

- `%` starts a conversion;
- whitespace causes input whitespace to be skipped;
- every other character must match the input literally.

It counts successful conversions in:

```c
int nconv;
```

The final return value is the number of successfully assigned values.

---

### `ft_scanf`

```c
int	ft_scanf(const char *format, ...);
```

This is the public function.

It:

1. creates a `va_list`;
2. starts it with `va_start`;
3. calls `ft_vfscanf` using `stdin`;
4. closes the argument list with `va_end`;
5. returns the conversion count.

```c
va_start(ap, format);
ret = ft_vfscanf(stdin, format, ap);
va_end(ap);
```

---

## Variadic arguments

`ft_scanf` accepts a variable number of destination arguments.

Example:

```c
ft_scanf("%d %s %c", &number, word, &letter);
```

The destination types must match the conversion:

```text
%d -> int *
%s -> char *
%c -> char *
```

Passing the wrong pointer type causes undefined behavior.

---

## Example test program

```c
#include <stdio.h>

int	ft_scanf(const char *format, ...);

int	main(void)
{
	int		number;
	char	word[100];
	char	letter;
	int		ret;

	ret = ft_scanf("%d %s %c", &number, word, &letter);
	printf("return: %d\n", ret);
	printf("number: %d\n", number);
	printf("word: %s\n", word);
	printf("letter: %c\n", letter);
	return (0);
}
```

Compile:

```bash
cc -Wall -Wextra -Werror ft_scanf.c main.c -o test_scanf
```

Run:

```bash
printf "42 hello Z\n" | ./test_scanf
```

Expected output:

```text
return: 3
number: 42
word: hello
letter: Z
```

---

## Additional tests

### Multiple integers

```c
int a;
int b;
int c;

ft_scanf("%d %d %d", &a, &b, &c);
```

Run:

```bash
printf "10 -20 30\n" | ./test_scanf
```

---

### Multiple strings

```c
char first[100];
char second[100];

ft_scanf("%s %s", first, second);
```

Input:

```text
hello world
```

---

### Characters without skipped spaces

```c
char a;
char b;
char c;

ft_scanf("%c%c%c", &a, &b, &c);
```

Input:

```text
abc
```

Results:

```text
a == 'a'
b == 'b'
c == 'c'
```

---

### Literal characters in the format

```c
int value;

ft_scanf("value=%d", &value);
```

Input:

```text
value=123
```

Result:

```text
value == 123
```

---

## Return values

The function normally returns the number of successful conversions.

Examples:

```text
3 -> three values were assigned
2 -> two values were assigned
1 -> one value was assigned
0 -> no conversion was completed
EOF -> input ended before a conversion could be completed
```

---

## Allowed functions used

This implementation uses only functions and macros allowed by the exercise:

```text
fgetc
ungetc
ferror
isspace
isdigit
stdin
va_start
va_arg
va_end
```

The headers are:

```c
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
```

---

## Limitations

This is intentionally a simplified version of `scanf`.

It does not support:

- conversion suppression with `*`;
- dynamic allocation with `m`;
- the `'` option;
- field widths;
- length modifiers such as `h`, `hh`, `l`, or `ll`;
- positional conversions such as `%1$d`;
- conversions other than `%s`, `%d`, and `%c`;
- buffer-size protection for `%s`.

It should not be used as a replacement for the complete standard-library `scanf` in production programs.

---

## Summary

The logic to remember is:

```text
1. Read the format one character at a time.
2. Skip input whitespace when the format contains whitespace.
3. Match normal format characters literally.
4. When `%` is found, choose c, d, or s.
5. Store each converted value through va_arg.
6. Count successful conversions.
7. Return the conversion count.
```
