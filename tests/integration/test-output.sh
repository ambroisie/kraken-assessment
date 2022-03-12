#!/usr/bin/env bash

if [ $# != 0 ]; then
    DATA_DIR="$1"
    KRAKEN="$2"
else
    DATA_DIR=data/
    KRAKEN=build/src/kraken
fi

if ! [ -x "$KRAKEN" ] || ! [ -d "$DATA_DIR" ]; then
    printf 'KRAKEN ('\''%s'\'') or DATA_DIR ('\''%s'\'') incorrectly set\n' "$KRAKEN" "$DATA_DIR"
    exit 1
fi

FAILURES=0
SUCCESSES=0

# $1: should be the name of the input/output files couple, stripped of
# the data path prefix, or the `{in,out}.csv` suffix.
test_file() {
    if ! diff="$(diff <("$KRAKEN" < "$DATA_DIR/inputs/$1.in.csv") "$DATA_DIR/outputs/$1.out.csv")"; then
        ((FAILURES += 1))
        echo "$1: FAIL"
        if [ -n "$VERBOSE" ]; then
            printf '%s\n' "$diff"
        fi
    else
        ((SUCCESSES += 1))
        echo "$1: OK"
    fi
}

for test_name in "$DATA_DIR"/inputs/*.in.csv; do
    test_name="$(basename "$test_name")"
    test_name="${test_name%%.in.csv}"

    test_file "$test_name"
done

printf '\nSummary: %d successes, %d failures\n' "$SUCCESSES" "$FAILURES"

exit "$((FAILURES != 0))"
