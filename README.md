makecorpus
==========

## Usage

```
$ for f in MIDI/*.txt
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

### Download MIDI files by scaping

Skip this step. MIDI text files are already ready.

```
$ gem install nokogiri
$ cd MIDI/
$ ../bin/scraping.rb
```

### Convert MIDI files to text format

```
$ for f in MIDI/*
do
    bin/mf2t -n $f corpus/`basename $f .mid`.txt
done
```
