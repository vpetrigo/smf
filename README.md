[![CI](https://github.com/vpetrigo/smf/actions/workflows/ci.yml/badge.svg)](https://github.com/vpetrigo/smf/actions/workflows/ci.yml)

# State Machine Framework

-------------------------
This project is a fork of a library from [Zephyr][zephyr] project which is great to use as a standalone library
for making state machines easier and cleaner.

The `smf` library has one of the cleanest interface and it is quite easy to use.

This repository provides an integration for the CMake build system that allows to add the `smf` library
to your project without a headache.

## Library configuration

There are two options available:
- `SMF_ANCESTOR_SUPPORT` enables/disables hierarchical state machine support
- `SMF_CONFIG_FILE` a configuration filename that will be included into `smf`

Configuration file may provide the following stuff that `smf` would use:

```c
#ifndef MY_SMF_CONFIG_H
#define MY_SMF_CONFIG_H

// enable hierarchical state machine feature
#define CONFIG_SMF_ANCESTOR_SUPPORT

// ...

// Logging macro that smf library uses
#define LOG_WARN(...)

#endif /* MY_SMF_CONFIG_H */
```

## Supported FSM types

_Flat state machine example_:

![flat_fsm](https://github.com/user-attachments/assets/57af1d45-0853-4f13-85fa-f77cb223be6c)


_Hierarchical state machine example (`CONFIG_SMF_ANCESTOR_SUPPORT` enabled)_:

![hierarchical_fsm](https://github.com/user-attachments/assets/df07612e-936f-4368-a939-a782485fdea9)


# Documentation

---------------

You may find the full-featured documentation
on [Zephyr project](https://docs.zephyrproject.org/latest/services/smf/index.html) page

# Contribution

--------------
Contributions are always welcome! If you have an idea, it's best to float it by me before working on it to ensure no
effort is wasted. If there's already an open issue for it, knock yourself out. See the [contributing section](CONTRIBUTING.md) for
additional details

# License

---------

This project is licensed under:

- [Apache-2.0](LICENSE.md)

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in time by you, as
defined in the Apache-2.0, shall be licensed as above, without any additional terms or
conditions.

[zephyr]: https://github.com/zephyrproject-rtos/zephyr
