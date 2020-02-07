#!/usr/bin/perl


$out = "";

%files;

while ($sor = <STDIN>) {
    if ($sor =~ /^\*\* Event /) {
        @items = split('   ', $sor);
        $out = $items[1];
        $out =~ s/^\s+|\s+$//g;
        $out =~ s/ .*$//;
#        print ">$out<\n";
        $sor =~ s|^\*\* Event \#\d+ |** Event #<eventNum> |;
        $sor = "\n$sor";
    }
    $sor =~ s|[0-9a-f]{4}-[0-9a-f]{4}/tplx|<fingerprint>|g;
    $files{$out} .= $sor;
}

while (my ($key, $value) = each %files) {
    $fn = "xxxsplit-$key.out";
#    print "fn:$fn\n";
    open(my $fh, ">", $fn)
        or die "Can't open > $fn: $!";

    print $fh $value;

    close($fh)
        || warn "close failed: $!";
}

