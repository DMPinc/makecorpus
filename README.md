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
