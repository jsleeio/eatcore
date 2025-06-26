// vim: ts=2 sw=2 expandtab
#include <stdio.h>
#include <stdlib.h>
#ifdef LINUX
#include <bsd/stdlib.h>
#endif
#include <unistd.h>
#include <strings.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#define logger(fmt, ...) fprintf(stderr, "%ld eatcore: " fmt "\n", (long) time(NULL), ## __VA_ARGS__)

#define VERSION "0.2.1"

struct context {
  char **argv;
  int argc;
  time_t interval;
  size_t baseline;
  size_t increment;
  size_t size;
  void *p;
  size_t max_increments;
  size_t current_increments;
  int exit_finished;
  int random;
};

void set_defaults(struct context *ctx, int argc, char **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
  ctx->interval = 60;
  ctx->increment = 150 * 1048576;
  ctx->baseline = 16 * 1048576;
  ctx->size = 0;
  ctx->p = NULL;
  ctx->max_increments = 10;
  ctx->current_increments = 0;
  ctx->exit_finished = 0;
  ctx->random = 0;
}

void parse_commandline(struct context *ctx)
{
  int ch;
  while ((ch = getopt(ctx->argc, ctx->argv, "b:hi:n:rs:xv")) != -1) {
    switch (ch) {
    case 's':
      ctx->increment = (size_t) atoi(optarg) * 1048576;
      break;
    case 'b':
      ctx->baseline = (size_t) atoi(optarg) * 1048576;
      break;
    case 'i':
      ctx->interval = (time_t) atoi(optarg);
      break;
    case 'n':
      ctx->max_increments = (size_t) atoi(optarg);
      break;
    case 'r':
      ctx->random = 1;
      break;
    case 'x':
      ctx->exit_finished = 1;
      break;
    case 'v':
      puts("version " VERSION);
      exit(0);
    case 'h':
    default:
      logger
          ("usage: eatcore [-i interval_in_seconds] [-b baseline_in_megabytes] [-s increment_in_megabytes] [-n max_increments] [-x] [-r]");
      exit(1);
    }
  }
  ctx->argc -= optind;
  ctx->argv += optind;
}

void increment(struct context *ctx, size_t increment)
{
  size_t newsize = ctx->size + increment;
  ctx->p = realloc(ctx->p, newsize);
  if (ctx->p) {
    ctx->size = newsize;
    logger("Stomach fed to %zu MB. Touching pages...", ctx->size / 1048576);
    if (ctx->random) {
      arc4random_buf(ctx->p, ctx->size);
    } else {
      bzero(ctx->p, ctx->size);
    }
    ctx->current_increments++;
  } else {
    fputs("Allocation failed, aborting.", stderr);
    exit(2);
  }
}

void eatcore(struct context *ctx)
{
  if (ctx->baseline != 0) {
    increment(ctx, ctx->baseline);
  }
  while (1) {
    if (ctx->current_increments > ctx->max_increments) {
      logger("Fully allocated.");
      return;
    }
    increment(ctx, ctx->increment);
    sleep(ctx->interval);
  }
}

int main(int argc, char **argv)
{
  struct context ctx;
  set_defaults(&ctx, argc, argv);
  parse_commandline(&ctx);
  logger
      ("Starting with interval=%d, baseline=%d, increment=%d, max_increments=%d, random=%d",
       (int) ctx.interval, (int) ctx.baseline, (int) ctx.increment,
       (int) ctx.max_increments, ctx.random);
  logger("Maximum allocation will be %d MB",
         (int) (ctx.baseline + ctx.max_increments * ctx.increment) / 1048576);
  eatcore(&ctx);
  if (ctx.exit_finished) {
    logger("Terminating.");
    exit(0);
  }
  logger("Sleeping.");
  while (1) {
    sleep(ctx.interval);
  }
}
