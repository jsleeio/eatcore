# `eatcore`

## What even is this?

`eatcore` is a tool intended to provoke memory allocation failures in
Kubernetes clusters. We were having some very strange problems with networking
under memory pressure and wanted to be able to reproduce the problems
on-demand.

Don't run this in your production Kubernetes cluster without running it
somewhere else first and observing the behaviour! If you really must, maybe
consider constraining it to some nodes that aren't running anything you care
about.

## How do I use it?

Included in this repo is an example Kubernetes `Deployment` definition, and
also a `Dockerfile`.

```
$ cat deployment.yaml ## understand what it will do first!
$ kubectl create -f deployment.yaml
```

### CLI options

`eatcore` has four key controls:

* `-b 96`: allocate+fill 96MB of memory once at startup

* `-s 500`: allocate+fill 500MiB of memory per cycle

* `-i 60`: sleep 60 seconds between cycles

* `-n 100`: quit after 100 cycles, if `-x` specified

Additionally, if `-x` is not specified, `eatcore` will sleep indefinitely after
completing its cycles.

If `-r` is specified, all memory allocated will be filled with data generated
by `arc4random`, rather than merely being filled with zeroes. This should
effectively prevent it being compressed or deduplicated by hypervisors or
operating system kernels.

## License

Copyright 2018 SiteMinder.

This software is released under the terms of the [MIT license](https://opensource.org/licenses/MIT).
