#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int	match_space(FILE *f)
{
	int	c;

	c = fgetc(f);
	while (c != EOF && isspace(c))
		c = fgetc(f);
	if (c != EOF)
		ungetc(c, f);
	else if (ferror(f))
		return (-1);
	return (1);
}


int	match_char(FILE *f, char e)
{
	int	c;

	c = fgetc(f);
	if (c == EOF)
		return (-1);
	if (c == e)
		return (1);
	ungetc(c, f);
	return (0);
}

int	scan_char(FILE *f, va_list ap)
{
	char	*dest;
	int		c;

	c = fgetc(f);
	if (c == EOF)
		return (-1);
	dest = va_arg(ap, char *);
	*dest = (char)c;
	return (1);
}


int	scan_int(FILE *f, va_list ap)
{
	int		*dest;
	int		c;
	int		sign;
	int		total;
	int		has_digit;

	sign = 1;
	total = 0;
	has_digit = 0;
	c = fgetc(f);
	if (c == EOF)
		return (-1);
	if (c == '-' || c == '+')
	{
		if (c == '-')
			sign = -1;
		c = fgetc(f);
	}
	while (c != EOF && isdigit(c))
	{
		total = total * 10 + (c - '0');
		has_digit = 1;
		c = fgetc(f);
	}
	if (c != EOF)
		ungetc(c, f);
	if (has_digit == 0)
		return (0);
	dest = va_arg(ap, int *);
	*dest = total * sign;
	return (1);
}

int	scan_string(FILE *f, va_list ap)
{
	char	*dest;
	int		c;
	int		i;

	c = fgetc(f);
	if (c == EOF)
		return (-1);
	dest = va_arg(ap, char *);
	i = 0;
	while (c != EOF && !isspace(c))
	{
		dest[i] = (char)c;
		i++;
		c = fgetc(f);
	}
	if (c != EOF)
		ungetc(c, f);
	if (i == 0)
		return (0);
	dest[i] = '\0';
	return (1);
}

int	match_conv(FILE *f, const char **format, va_list ap)
{
	switch (**format)
	{
		case 'c':
			return scan_char(f, ap);
		case 'd':
			match_space(f);
			return scan_int(f, ap);
		case 's':
			match_space(f);
			return scan_string(f, ap);
		case EOF:
			return -1;
		default:
			return -1;
	}
}

int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int nconv = 0;

	int c = fgetc(f);
	if (c == EOF)
		return EOF;
	ungetc(c, f);

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (match_conv(f, &format, ap) != 1)
				break;
			else
				nconv++;
		}
		else if (isspace(*format))
		{
			if (match_space(f) == -1)
				break;
		}
		else if (match_char(f, *format) != 1)
			break;
		format++;
	}
	
	if (ferror(f))
		return EOF;
	return nconv;
}


int ft_scanf(const char *format, ...)
{
	va_list ap;
    va_start(ap,format);
	int ret = ft_vfscanf(stdin, format, ap);
	va_end(ap);
	return ret;
}
