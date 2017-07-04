#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>

#define logger(fmt, ...) fprintf(stderr, "eatcore: " fmt "\n", ## __VA_ARGS__)

#define VERSION "0.1.3"

struct context {
	char** argv;
	int    argc;
	time_t interval;
	size_t increment;
	size_t size;
	void*  p;
  size_t max_increments;
  size_t current_increments;
  int    exit_finished;
};

void set_defaults(struct context* ctx, int argc, char** argv) {
  ctx->argc               = argc;
  ctx->argv               = argv;
  ctx->interval           = 60;
  ctx->increment          = 150*1048576;
  ctx->size               = 0;
  ctx->p                  = NULL;
  ctx->max_increments     = 10;
  ctx->current_increments = 0;
  ctx->exit_finished      = 0;
}

void parse_commandline(struct context* ctx) {
  int ch;
	while ((ch = getopt(ctx->argc, ctx->argv, "hi:n:s:xv")) != -1) {
		switch (ch) {
			case 's':
				ctx->increment = (size_t) atoi(optarg) * 1048576;
				break;
			case 'i':
				ctx->interval = (time_t) atoi(optarg);
				break;
      case 'n':
        ctx->max_increments = (size_t) atoi(optarg);
        break;
      case 'x':
        ctx->exit_finished = 1;
        break;
      case 'v':
        puts("version " VERSION);
        exit(0);
      case 'h':
      default:
        logger("usage: eatcore [-i interval_in_seconds] [-s increment_in_bytes] [-n max_increments] [-x]");
        exit(1);
		}
	}
	ctx->argc -= optind;
	ctx->argv += optind;
}

void interval(struct context* ctx) {
  if (ctx->exit_finished && ctx->current_increments >= ctx->max_increments) {
    logger("Fully allocated. Exiting.");
    exit(0);
  } else {
    logger("Sleeping...");
    sleep(ctx->interval);
  }
}

void increment(struct context* ctx) {
  size_t newsize = ctx->size + ctx->increment;
  ctx->p = realloc(ctx->p, newsize);
  if (ctx->p) {
    ctx->size = newsize;
    logger("Stomach fed to %zuMB. Touching pages...", ctx->size/1048576);
    bzero(ctx->p, ctx->size);
    logger("Finished touching pages. Sleeping %d seconds.", (int) ctx->interval);
    ctx->current_increments++;
  } else {
    fputs("Allocation failed, aborting.", stderr);
    exit(2);
  }
}

void eatcore(struct context* ctx) {
	while (1) {
    if (ctx->current_increments < ctx->max_increments)
      increment(ctx);
    interval(ctx);
  }
}

int main(int argc, char** argv) {
	struct context ctx;
	set_defaults(&ctx,argc,argv);
	parse_commandline(&ctx);
	logger("Starting with interval=%d, increment=%d, max_increments=%d",
    (int) ctx.interval, (int) ctx.increment, (int) ctx.max_increments);
	eatcore(&ctx);
}
