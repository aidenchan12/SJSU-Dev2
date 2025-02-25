#include "newlib/newlib.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "L0_Platform/ram.hpp"
#include "utility/macros.hpp"

namespace sjsu
{
namespace newlib
{
int DoNothingStdOut(const char *, size_t)
{
  return 1;
}
int DoNothingStdIn(char *, size_t)
{
  return 0;
}

Stdout out                = DoNothingStdOut;
Stdin in                  = DoNothingStdIn;
bool echo_back_is_enabled = true;

void SetStdout(Stdout stdout_handler)
{
  out = stdout_handler;
}
void SetStdin(Stdin stdin_handler)
{
  in = stdin_handler;
}
void StdinEchoBack(bool enable_echo)
{
  echo_back_is_enabled = enable_echo;
}
}  // namespace newlib
}  // namespace sjsu

extern "C"
{
  // Dummy implementation of isatty
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _isatty([[maybe_unused]] int file)
  {
    return 1;
  }
  // Dummy implementation of exit with return code placed into
  // Arm register r3
  // NOLINTNEXTLINE(readability-identifier-naming)
  void _exit([[maybe_unused]] int rc)
  {
    while (1)
    {
      continue;
    }
  }
  // Dummy implementation of getpid
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _getpid()
  {
    return 1;
  }
  // Dummy implementation of kill
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _kill(int, int)
  {
    return -1;
  }
  // Dummy implementation of fstat, makes the assumption that the "device"
  // representing, in this case STDIN, STDOUT, and STDERR as character devices.
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _fstat([[maybe_unused]] int file, struct stat * status)
  {
    status->st_mode = S_IFCHR;
    return 0;
  }
  // Implementation of sbrk for allocating and deallocating space for malloc.
  // NOLINTNEXTLINE(readability-identifier-naming)
  void * _sbrk(int increment)
  {
    void * previous_heap_position = static_cast<void *>(heap_position);
    // Check that by allocating this space, we do not exceed the heap area.
    if ((heap_position + increment) > &heap_end)
    {
      previous_heap_position = nullptr;
    }
    heap_position += increment;
    return previous_heap_position;
  }
  // Dummy implementation of close
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _close([[maybe_unused]] int file)
  {
    return -1;
  }
  // Minimum implementation of _write using UART0 putchar
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _write([[maybe_unused]] int file, char * ptr, int length)
  {
    return sjsu::newlib::out(ptr, length);
  }
  // Dummy implementation of _lseek
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _lseek([[maybe_unused]] int file, [[maybe_unused]] int ptr,
             [[maybe_unused]] int dir)
  {
    return 0;
  }
  // Minimum implementation of _read using UART0 getchar
  // NOLINTNEXTLINE(readability-identifier-naming)
  int _read(FILE * file, char * ptr, [[maybe_unused]] int length)
  {
    int number_of_read_characters = 0;
    if (file == STDIN_FILENO)
    {
      number_of_read_characters = sjsu::newlib::in(ptr, 1);
      // Echo back to STDOUT
      if (sjsu::newlib::echo_back_is_enabled)
      {
        sjsu::newlib::out(ptr, 1);
      }
    }
    return number_of_read_characters;
  }
  // Needed by third party printf library
  void _putchar(char character)  // NOLINT
  {
    sjsu::newlib::out(&character, 1);
  }

  // Overload default libnano putchar() with a more optimal version that does
  // not use dynamic memory
  int putchar(int character)  // NOLINT
  {
    char character_value = static_cast<char>(character);
    return sjsu::newlib::out(&character_value, 1);
  }

  // Overload default libnano puts() with a more optimal version that does
  // not use dynamic memory
  int puts(const char * str)  // NOLINT
  {
    size_t string_length = strlen(str);
    int result           = 0;
    result += sjsu::newlib::out(str, string_length);
    result += sjsu::newlib::out("\n", 1);
    // + 1 because puts adds an additional newline '\n' character.
    return result;
  }
}

#pragma GCC diagnostic ignored "-Wformat-nonliteral"
// The newlib nano version of scanf relies on malloc.
// Overriding scanf with an efficient static memory variant.
// NOLINTNEXTLINE(readability-identifier-naming)
int scanf(const char * format, ...)
{
  va_list args;
  va_start(args, format);
  int items = vfscanf(stdin, format, args);
  va_end(args);
  return items;
}
#pragma GCC diagnostic warning "-Wformat-nonliteral"
