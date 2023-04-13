# Triceratops-XT

Triceratops is a virtual subtractive synthesizer plugin with 3 oscillators, 3 ADSR envelopes and 3 LFOs. Oscillators have unison mode, filter has three modes low pass resonant 24db, high pass and band pass. Oscillator sync, inertia (portamento) and basic FM is implemented. 

Now Triceratops is implemented as a cross-platform audio plugin by AnClark Liu. To distinguish, this project is called **Triceratops-XT**.

> **NOTICE: UI is not yet implemented!**

## Source Tree

- `plugin`: Plugin interface.
- `src`: Source tree of upstream containing synth engine. Introduced as a submodule.
- `dpf`: DISTRHO Plugin Framework source tree. Introduced as a submodule as well. 

## Dependencies

The only dependency is DISTRHO Plugin Framework.

## Compile

```bash
# Clone this repo
git clone https://github.com/AnClark/Triceratops-XT.git --recursive
cd Triceratops-XT

# In case you forgot the `--recursive` switch above
git submodule update --init --recusrive

# Now let's build!
cmake -S . -B build
cmake --build build
```

Built plugins reside in `build/bin/`.

Optionally, you can also build the original LV2 plugin. Refer to `src/README.md` for more details.

## Run

Triceratops-XT supports those plugin formats:

- VST 2.4
- VST3
- LV2
- CLAP

Just copy plugin directories or files to your DAW's scan pathes, then re-scan plugins in your DAW.

## License

Triceratops-XT is licensed under GPLv3.
