#!/usr/bin/perl
# ./translate.pl "brainfuck program" > program.heu
{
	$ARGV[0] =~ s/>/Heu !/g;
	$ARGV[0] =~ s/</Heu ?/g;
#Beware the following replacement contains characters of the source set
	$ARGV[0] =~ s/,/Heu, t'en penses quoi ?/g;
	$ARGV[0] =~ s/\./Heu, j'avais pensé que./g;
	$ARGV[0] =~ s/\+/Heu.../g;
	$ARGV[0] =~ s/-/Heu./g;
	$ARGV[0] =~ s/\[/Heu, heu!/g;
	$ARGV[0] =~ s/\]/Heu, heu?/g;
	
	print $ARGV[0];
}