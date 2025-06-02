#ifndef __LOG_H__
#include <stdlib.h>
#define EvalPrintD(x) printf("%s = %d\n", #x, (x));
#define EvalPrintZU(x) printf("%s = %zu\n", #x, (x));

#define LOG_UNREACHABLE(message) do{fprintf(stderr, "%s:%d UNREACHABLE: %s\n", __FILE__, __LINE__, message); abort();} while(0);

typedef enum{
  INFO,
  WARNING,
  ERROR,
  NO_LOGS,
} log_level_e;

log_level_e minimal_log_level = INFO;

void Log( log_level_e Level, const char *Format, ...)
{
  if(Level < minimal_log_level){
    return;
  }
  switch(Level) {
    case INFO:{
      fprintf(stderr, "[INFO]: ");
    }break;

    case WARNING:{
      fprintf(stderr, "[WARNING]: ");
    }break;

    case ERROR:{
      fprintf(stderr, "[ERROR]: ");
    }break;

    case NO_LOGS:{
      return;
    }

    default:{
      LOG_UNREACHABLE("log");
    }
  }

  va_list Arguments;
  va_start(Arguments, Format);
  vfprintf(stderr, Format, Arguments);
  va_end(Arguments);
  fprintf(stderr,"\n");
}

void LogFatal( log_level_e Level, const char *Format, ...)
{
  if(Level < minimal_log_level){
    return;
  }
  switch(Level) {
    case INFO:{
      fprintf(stderr, "[INFO]: ");
    }break;

    case WARNING:{
      fprintf(stderr, "[WARNING]: ");
    }break;

    case ERROR:{
      fprintf(stderr, "[ERROR]: ");
    }break;

    case NO_LOGS:{
      return;
    }

    default:{
      LOG_UNREACHABLE("log");
    }
  }

  va_list Arguments;
  va_start(Arguments, Format);
  vfprintf(stderr, Format, Arguments);
  va_end(Arguments);
  fprintf(stderr,"\n");
  exit(21);
}
#define __LOG_H__
#endif
