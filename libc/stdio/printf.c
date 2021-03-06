#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		}
		else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		}
		else if (*format == 'l') {
			format++;
			// TODO: 'l' shouldnt be long long. I'm doing this because I'm lazy.
			long long val = va_arg(parameters, long long);
			char cbuf[24];
			itoa64(val, cbuf, 24, 10);
			size_t len = strlen(cbuf);
			if(maxrem < len) {
				// TODO: Set errno to EOVERFLOW
				return -1;
			}
			if(!print(cbuf, len))
				return -1;
			written += len;
		}
		else if (*format == 'd') {
			format++;
			long val = va_arg(parameters, long);
			char cbuf[24];
			itoa(val, cbuf, 24, 10);
			size_t len = strlen(cbuf);
			if(maxrem < len) {
				// TODO: Set errno to EOVERFLOW
				return -1;
			}
			if(!print(cbuf, len))
				return -1;
			written += len;
		}
		else if (*format == 'p') {
			format++;
			void* val = va_arg(parameters, void*);
			char cbuf[9];
			itoa((int)val, cbuf, 9, 16);
			size_t len = strlen(cbuf);

			//TODO: This padding array would need to be extended if I would ever go 64-bit
			char padding[] = {'0', '0', '0', '0', '0', '0', '0', '\0'};
			//sizeof(void*)*2 is the maximum size of a pointer hex string.
			size_t padcount = (sizeof(void*) * 2) - len;
			padding[padcount] = '\0';

			if(maxrem < sizeof(void*) * 2) {
				// TODO: Set errno to EOVERFLOW
				return -1;
			}
			if(!print(padding, padcount))
				return -1;
			if(!print(cbuf, len))
				return -1;
			written += len;
		}
		else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
