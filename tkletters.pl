#!/usr/bin/perl
# Tk interface to the cntdn letters solver
# James Stanley 2011

use Tk;
use IPC::Open2;

use strict;
use warnings;

my ($childin, $childout, $display);

# Handle a key press by updating the display
sub keypress {
    my ($input) = @_;

    my @words = ();

    # don't get any capital letters
    $input = lc $input;

    # give the letters to the solver
    print $childin "$input\n";

    # read words from the solver, adding them to @words
    my $line = <$childout>;
    while ($line ne "\n") {
        push(@words, $line);

        $line = <$childout>;
    }

    # sort @words by length
    @words = sort { length( $b ) <=> length( $a ) } @words;

    # set the text in the display to be the words
    $display->configure( -state => 'normal' );
    $display->delete( '1.0', 'end' );
    $display->insert( '1.0', (join '', @words) );
    $display->configure( -state => 'disabled' );

    return 1;
}

# make a Tk window
my $mw = MainWindow->new();

# make the letter input box
my $input = $mw->Entry( -validate => 'key', -vcmd => \&keypress );

# make the scrollbar
my $scrollbar = $mw->Scrollbar( -orient => 'vertical' );

# make the area displaying the solution words
$display = $mw->Text( -width => 25, -height => 20, -state => 'disabled',
        -yscrollcommand => ['set' => $scrollbar] );

# make the scrollbar control the display
$scrollbar->configure( -command => ['yview' => $display] );

# pack widgets in to window
$input->pack( -fill => 'x' );
$scrollbar->pack( -side => 'right', -fill => 'y');
$display->pack( -expand => 1, -side => 'left', -fill => 'both' );

# spawn a letters game solver
my $pid = open2( $childout, $childin, 'letters', '-I', '-m0' );

# start the Tk main loop
MainLoop();

# let the solver exit
close $childin;
waitpid( $pid, 0 );
