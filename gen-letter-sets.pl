#!/usr/bin/perl
# Generate letter sets for countdown
# James Stanley 2011

use strict;
use warnings;

my @LETTERS = split( //, 'abcdefghijklmnopqrstuvwxyz' );

my $sets = shift or die "usage: gen-letter-sets N\n";

while ($sets--) {
    for (my $i = 0; $i < 9; $i++) {
        print $LETTERS[rand @LETTERS];
    }
    print "\n";
}
