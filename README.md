# `eatcore`

## What even is this?

`eatcore` is a tool intended to provoke memory allocation failures in
Kubernetes clusters. We were having some very strange problems with networking
under memory pressure and wanted to be able to reproduce the problems
on-demand.

## How do I use it?

Included in this repo is an example Kubernetes `Deployment` definition, and
also a `Dockerfile`. As we've not yet pushed an image anywhere, you will need
to push one to your own repository and edit the definition to point at it.

### CLI options

`eatcore` has three key controls:

* `-s 500`: allocate+fill 500MiB of memory per cycle

* `-i 60`: sleep 60 seconds between cycles

* `-n 100`: quit after 100 cycles, if `-x` specified

Additionally, if `-x` is not specified, `eatcore` will sleep indefinitely after
completing its cycles.

## Potential improvements

### Defeat memory compression/deduplication regimes

Some hypervisors try to compress or deduplicate memory. An attempt could be
made to defeat such mechanisms by filling pages with random garbage instead of
a simple `bzero`.
