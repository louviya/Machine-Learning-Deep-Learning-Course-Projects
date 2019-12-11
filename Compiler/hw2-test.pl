#!/usr/bin/perl -w

# Perl script for testing CSCE 531 hw2 submissions (Simple Expr Eval)

# Usage (must be run under the bash shell):
# $ hw2-test.pl --self-test <your_hw2_directory>
#
# For example:
# $ hw2-test.pl --self-test hw2

# Appends to file "comments.txt" in your directory

# This script must be run under the bash shell!

# edit as needed
$testSuiteDir = "$ENV{HOME}/test";

$submission_root = "$ENV{HOME}/courses/csce531/sp15/hw2/submissions";
$timeout = 11;			# seconds
$prog_name = 'see';
$altProgCount = 2;              # the number of alternate versions of the prog

# HW2-specific (edit and expand with additional test files as desired)
%test_files =
    (
     'see'  => [
	 'p4-correct1',
	 'p4-correct2',
	 'p4-error1',
	 'p4-error2',
	 'p4-error3',
	 'p4-error4',
	 'p4-error5',
	 'p4-error6',
	 'p4-error7',
	 'p5-untimed1',
	 'p5-error1',
	 'p5-error2',
	 'p5-error3',
	 'p6-timed1',
	 'p6-timed2',
	 'p6-timed3',
	 'p7-untimed1',
	 'p7-timed1',
	 'p7-error1',
	 'p7-error2',
	 'p7-error3',
	 'p7-error4'
     ],
     'see1' => [
	 'p4-correct1',
	 'p4-correct2',
	 'p4-error1',
	 'p4-error2',
	 'p4-error3',
	 'p4-error4',
	 'p4-error5',
	 'p4-error6',
	 'p4-error7'
     ],
     'see2' => [
	 'p5-untimed1',
	 'p5-error1',
	 'p5-error2',
	 'p5-error3',
	 'p6-timed1',
	 'p6-timed2',
	 'p6-timed3',
	 'p7-untimed1',
	 'p7-timed1',
	 'p7-error1',
	 'p7-error2',
	 'p7-error3',
	 'p7-error4'
     ]
    );

# This varies by system
# On CSE Linux systems
$bison_parse_error_msg = "syntax error";
# On my iMac
#$bison_parse_error_msg = "parse error";

############ You should not have to edit below this line ##############

# Hash to hold the test results
%error_counts = ();
%strip_error_counts = ();

# Create command versions of directories (escape spaces in pathnames)
# $cmdSubRoot = $submissionRoot;
# $cmdSubRoot =~ s/ /\\ /g;
$cmdTestSuiteDir = $testSuiteDir;
$cmdTestSuiteDir =~ s/ /\\ /g;

# Check existence of test suite directory
die "No test suite directory $testSuiteDir\n"
    unless -d $testSuiteDir;

#sub main
{
    if (!@ARGV) {
	print "Usage:\n  hw2-test.pl --self-test <your_hw2_directory>\n\n";
	print "For example:\n  \$ ./hw2-test.pl --self-test hw2\n\n";
	print "This program must be invoked from the bash shell.\n";
	print "When in doubt, run \"bash\" first, then \"exit\" after.\n\n";
    }
    elsif ($ARGV[0] eq '--self-test') {
	shift @ARGV;
	die "hw2-test.pl: missing directory name\n"
	    unless @ARGV;

	$hw2_dir = shift @ARGV;

	if ($hw2_dir !~ /^(~|\/)/) {
	    # relative path name -- prepend pwd
	    $pwd = `pwd`;
	    chomp $pwd;
	    $hw2_dir = "$pwd/$hw2_dir";
	}
	# convert home directory-relative pathname
	$hw2_dir =~ s/^~/$ENV{HOME}/e;
	# strip off final slash, if any
	$hw2_dir =~ s/\/$//;

	$uname = 'self-test';
	process_user();
    }
    elsif ($ARGV[0] eq '--grading') {
	# using for grading -- optionally give user name(s)
	shift @ARGV;
	if (@ARGV) {
	    # process individually specified user(s)
	    while (@ARGV) {
		$uname = shift @ARGV;
		$hw2_dir = "$submission_root/$uname/hw2";
		die "No such directory: $hw2_dir\n"
		    unless -d $hw2_dir;
		process_user();
	    }
	}
	else {
	    opendir DIR, $submission_root
		or die "Cannot open submission directory $submission_root ($!)\n";
	    @usernames = readdir DIR;
	    closedir DIR;

	    while (@usernames) {
		$uname = shift @usernames;
		next if $uname =~ /^\./;
		$hw2_dir = "$submission_root/$uname/hw2";
		next unless -d $hw2_dir;
		process_user();
	    }
	}
    }
}


sub process_user {
    print(STDERR "Processing $uname\n\n");
    die "No hw2 directory corresponding to $uname ($!)\n"
	unless -d $hw2_dir;

    open(COMMENTS, "> $hw2_dir/comments.txt");

    cmt("Comments for $uname -------- " . now() . "\n");

    chdir $hw2_dir
	or die "Cannot change to directory $hw2_dir ($!)\n";

    # try make clean, regardless of what happens
    system("make", "clean");

    opendir DIR, $hw2_dir
	or die "Cannot open $hw2_dir directory ($!)\n";
    @filenames = readdir DIR;
    closedir DIR;
    #    cmt("    Extra files found:");
    $count = 0;
    while (@filenames) {
	$filename = shift @filenames;
	chomp $filename;
	next if $filename =~ /^\./;
	if ($filename =~ /lex\.yy|y\.tab|\.o$|parse\.c|scan\.c|see[12]?/) {
	    cmt("Removing illegal file: $filename\n");
	    unlink $filename;
	    $count++;
	}
#    else {
#      # file is ok -- "touch" it just in case ...
#      $atime = $mtime = time;
#      utime $atime, $mtime, ($filename)
#	or die "Cannot update mod time on $filename ($!)\n";
#    }
    }
    cmt("$count illegal files found\n");

    test_prog($prog_name);

    report_summary();

    close COMMENTS;

    # try cleaning -- don't care what happens
    system("make", "clean");

    print(STDERR "\nDone.\nComments are in $hw2_dir/comments.txt\n");
}


sub test_prog {
    my ($prog) = @_;

    print(STDERR "Errors msgs for $uname/$prog:\n");
    print(STDOUT "System msgs for $uname/$prog:\n");

    cmt("Testing $prog:\n");

    $error_counts{$prog} = 0;
    $strip_error_counts{$prog} = 0;

    $rc = test_make($prog);

    if (!$rc && $prog !~ /\d+$/) { # if couldn't make original prog
	cmt("Make of $prog failed; testing alternate versions\n");
	$error_counts{$prog}++;
	for ($cnt = 1; $cnt <= $altProgCount; $cnt++) {
	    test_prog("$prog$cnt");
	}
	return;
    }

    print(STDERR "Error msgs for $uname/$prog:\n");

    foreach $base (@{$test_files{$prog}}) {
	-e "$testSuiteDir/$base.in" || die "$base.in does not exist ($!)\n";
	cmt("Running $prog with input $base.in ...");
	print(STDERR "----$base.in:\n");
	$testFile = "$cmdTestSuiteDir/$base.in";
	unlink "$base.out"
	    if -e "$base.out";
	$error_counts{$base} = 0;
	# assume stripped files don't match unless we find that they do
	$strip_error_counts{$base} = 1;
	if ($base =~ /-timed/) {
	    eval {
		local $SIG{ALRM} = sub { die "TIMED OUT\n" };
		alarm $timeout;
		$rc = system("./$prog < $testFile > $base.out 2> $base.err");
		alarm 0;
	    };
	    if ($@ && $@ eq "TIMED OUT\n") {
		cmt(" $@");		# program timed out before finishing
		$error_counts{$base}++;
		next;
	    }
	}
	else {
	    $rc = system("./$prog < $testFile > $base.out 2> $base.err");
	}
	if ($rc >> 8) {
	    cmt(" nonzero termination status\n");
	}
	else {
	    cmt(" zero termination status\n");
	}

	# if error test, don't care about output; check for error message instead
	if ($base =~ /error/) {
	    if (!(-e "$base.err")) {
		cmt("  ERROR MESSAGE FILE DOES NOT EXIST\n");
		$error_counts{$base}++;
		unlink "$base.out"
		    if -e "$base.out";
		next;
	    }
	    $errmsg = `cat $base.err`;
	    unlink "$base.err";
	    chomp $errmsg;
	    if ($errmsg eq $bison_parse_error_msg) {
		cmt("  Error message matches bison's normal parse error message");
		if ($base =~ /^p4/) {    # problem 4 errors are all syntax errors
		    cmt(" (ok)\n");
		}
		else {          # no syntax errors in other problems besides 4
		    cmt(" (INCORRECT)\n");
		    $error_counts{$base}++;
		}
	    }
	    else {
		cmt("  Contents of file $base.err:");
		if ($errmsg !~ /\w/s) {
		    cmt(" (BLANK):");
		    $error_counts{$base}++;
		}
		cmt("\n\n\"$errmsg\"\n\n");
		$error_counts{$base}++
		    if $base =~ /^p4/;    # syntax error should be only problem 4 error
	    }
	    next;
	}

	if (!(-e "$base.out")) {
	    cmt("  OUTPUT FILE $base.out DOES NOT EXIST\n");
	    $error_counts{$base}++;
	    next;
	}

	cmt("  $base.out exists\n  Testing output against solution with diff ...");
	$report = `diff $base.out $cmdTestSuiteDir/$base.out`;
	chomp $report;
	if ($report eq '') {
	    cmt(" files match (ok)\n");
	    unlink "$base.out";
	    next;
	}
	cmt(" FILES DIFFER:\n$report\n");
	$error_counts{$base}++;

	cmt("    Retry after stripping nonessentials from output ...");
	$myOut = `cat $cmdTestSuiteDir/$base.out`;
	$yourOut = `cat $base.out`;
	unlink "$base.out";
	# Strip out everything but the final number on each line
	$myOut =~ s/^[^\n0-9-]*(?:[0-9-]+[^\n0-9-]+)*([-]?\d+)[ \t]*$/$1/mg;
	$yourOut =~ s/^[^\n0-9-]*(?:[0-9-]+[^\n0-9-]+)*([-]?\d+)[ \t]*$/$1/mg;
	# strip lines not containing numbers
	$myOut =~ s/^.*[^\n0-9-].*$//mg;
	$yourOut =~ s/^.*[^\n0-9-].*$//mg;
	# Strip any blank lines
	$myOut =~ s/^[ \t]*\n//s;
	$myOut =~ s/\n[ \t]*\n/\n/sg;
	$yourOut =~ s/^[ \t]*\n//s;
	$yourOut =~ s/\n[ \t]*\n/\n/sg;
	chomp $myOut;
	chomp $yourOut;
	if ($myOut ne $yourOut) {
	    cmt(" STRIPPED TEXTS STILL DIFFER\n");
	    cmt("\n  myOut = \"\n$myOut\n\"");
	    cmt("\n  yourOut = \"\n$yourOut\n\"\n");
	}
	else {
	    cmt(" stripped files match (ok)\n");
	    $strip_error_counts{$base} = 0;
	}
	unlink "$base.out"
	    if -e "$base.out";
        unlink "$base.err"
	    if -e "$base.err";
    }
}


# Tries to make the given executable program.  Returns true iff success
sub test_make {
    my ($prog) = @_;

    cmt("  Attempting to build $prog ...");
    $rc = system("make", $prog);
    if ($rc >> 8) {
	cmt(" make $prog FAILED\n");
	return 0;
    }

    if (!(-e $prog)) {
	cmt(" $prog EXECUTABLE DOES NOT EXIST\n");
	return 0;
    }

    cmt(" ok\n");
    return 1;
}


sub report_summary {
    cmt("######################################################\n");
    cmt("Summary for $uname:\n");

    foreach $item (sort(keys %error_counts)) {
	cmt("  $item: ");
	$cnt = $error_counts{$item};
	if ($cnt > 0) {
	    cmt(" problem(s) found");
	    if ($strip_error_counts{$item} > 0) {
		cmt(" (AND NO STRIPPED OUTPUT MATCH)");
	    }
	    else {
		cmt(" (but no problem with stripped outputs)");
	    }
	}
	elsif ($item !~ /error/) {
	    cmt(" ok");
	}
	elsif ($item =~ /^p4/) {
	    cmt(" error message ok");
	}
	else {
	    cmt(" error message(s) (check appropriateness)");
	}
	cmt("\n");
    }
    cmt("######################################################\n");
    cmt("Problem 3: /30\n");
    cmt("Problem 4: /30\n");
    cmt("Problem 5: /40\n");
    cmt("Problem 6: /20\n");
    cmt("Problem 7: /30\n");
    cmt("Problem X: \n");
}


sub cmt {
    my ($str) = @_;
#  print $str;
    print(COMMENTS $str);
}


sub now {
    my $ret;

    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime;
    $ret = ('Sun','Mon','Tue','Wed','Thu','Fri','Sat')[$wday];
    $ret .= " ";
    $ret .= ('Jan','Feb','Mar','Apr','May','Jun','Jul',
	     'Aug','Sep','Oct','Nov','Dec')[$mon];
    $ret .= " $mday, ";
    $ret .= $year + 1900;
    $ret .= " at ${hour}:${min}:${sec} ";
    if ( $isdst ) {
	$ret .= "EDT";
    } else {
	$ret .= "EST";
    }
    return $ret;    
}
