# dummy_thicc

A collaborative effort to build the fattest Mach-O binary in existence.

## Architectures

## Contributing

Think the list above is missing a slice? Happen to have some obscure hardware lying around that can run NeXTSTEP or macOS? There are a couple ways you can help, but first, an important caveat: at any given time, every slice in the `dummy_thicc` universal binary, and each `dummy_thicc` thin slice, must actually run somewhere–virtually or physically. This has two important consequences: one, some slices, while ostensibly recognized by Mach-O tooling as valid, will not be permitted as there was anything that could run it. Second, every time the binary is updated it must be retested on every supported platform. Keeping this in mind, here's the steps to a add a new slice:

1. Compile dummy_thicc.c for your target platform. When possible, it is preferred that the compiler invocation has the flags `-Os -DARCH=\"$NEW_ARCHITECTURE\" -arch $NEW_ARCHITECTURE dummy_thicc.c -o $NEW_ARCHITECTURE/dummy_thicc`.
2. Test that the resultant binary works by itself.
3. Use `lipo -create */dummy_thicc -output dummy_thicc` to create a new universal binary. Test that this binary works on `$NEW_ARCHITECTURE`.
4. Update the README. (Canonical architecture names can be found using `NXGetAllArchInfos`.)
5. Send a pull request with your changes. Everyone else will then test the new binary on their system.
6. If the universal binary works on every other system, then `$NEW_ARCHITECTURE` will be added. Success!

### Testing

Testing `dummy_thicc` is a little complicated. The binary itself is simple: each slice prints out "Hello from $ARCHITECTURE!". Slices have two components to them: a CPU type and a CPU subtype. CPU types are used to distinguish different architectures, while CPU subtypes distinguish between microarchitectures. Most processors support multiple microarchitectures but a single main CPU architecture, but some might support multiple CPU types. In addition, some kernels might support running binaries of other systems in emulation. As such, a single system might support multiple slices of the same binary. Testing a specific slice of the universal binary:

* Must actually use the full universal binary–no stripping out problematic slices, or using the slice by itself.
* Must actually test the specific slice. For example, running the universal binary and seeing "Hello from x86_64!" does not mean that you've tested x86_64h, and seeing "Hello from x86_64h!" does not count for x86_64.
* Can be tested in a virtualized or emulated environment, but the Mach loader in the kernel may not be modified–it can only be influenced via publicly accessibly APIs. This means you can run in a virtual machine or emulator to test the slice, or a built-in binary emulator such as Rosetta. If there is API to pick a slice, you may use that too–there are tools to do that in the [utils](https://github.com/saagarjha/dummy_thicc/tree/master/utils) directory, although the APIs use may not be precise enough to select a particular subtype. In that case you will have to use a machine with a specific CPU that will cause the OS to preferentially choose that slice. When testing, mild preference will be given towards real hardware running its native slice, then one that is selecting another one using tools, then one using inbuilt emulation, and then finally a fully virtualized system.
