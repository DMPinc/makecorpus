makecorpus
==========

## Training data: training.txt

MIDI files from http://piano.s20.xrea.com/midi/main.html

## Usage

```
$ for f in corpus/*.txt
do
    bin/makecorpus $f >> traing.txt
done
```

## Output Format

Length/Sound

## Example

- 単音：432/e4
- 和音：216/{e6,g4,d6}
- 無音：12/sp

## Optional

You can skip this step. MIDI files and MIDI text files are already ready.

### Download MIDI files by scaping

```
$ gem install nokogiri
$ cd MIDI/
$ ../bin/scraping.rb
```

### Convert MIDI files to text format

mf2t is accessible from
https://code.google.com/p/midi2text/downloads/list.

```
$ for f in MIDI/*
do
    bin/mf2t -n $f corpus/`basename $f .mid`.txt
done
```
