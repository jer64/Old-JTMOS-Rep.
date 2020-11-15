#
################################################################
my (@web);

#
#use Time::localtime;
#use File::stat;
#use CGI;
#use DBI;

#
$CACHE_STAT_INTERVAL = (60*60*1);
#
$GLOBALW =  "100%";
$GLOBALW2 = "100%";
$UUSI_IKA = 60*60*24;
#
@iopt_keys = ("specialreport", "icap", "ontop", "imageurl", "imageurl2", "imageurl3", "imageurl4",
                "NO_FLY_FIX", "full", "dontembed", "COMMENTS_ENABLED",
                "swfurl", "quote_loc", "w_below_cap",
		"geoloc");

#
################################################################
# Your home computer's IP (for master access in some cases).
$MASTER_IP=             "82.103.200.47";
#
@MASTER_IPS=(
	#	"82.103.200.47",
	#	"82.103.201.139"
		);
# Remove remark below to disable the search service for public users.
$SDB_SERVICE_OFF = 0;
#
$ARTBASE = "$ENV{'DOCUMENT_ROOT'}/articles";
$IMAGES_BASE = "/images";
$NWPUB_WWWBASE =        "$ENV{'DOCUMENT_ROOT'}";
$NWPUB_CGIBASE =        "$ENV{'DOCUMENT_ROOT'}/cgi";
$ENV{'NWPUB_WWWBASE'} = $NWPUB_WWWBASE;
$ENV{'NWPUB_CGIBASE'} = $NWPUB_CGIBASE;


#
$VAI_PROF_DIR = "$NWPUB_CGIBASE/user/";
$IMGBASE = "$IMAGES_BASE";
$NEBASE = "$IMAGES_BASE";
$NEBASECGI = "/cgi";
#
$BGBAK1 = "$IMAGES_BASE/bakki5.gif";
$BGBAK2 = "$IMAGES_BASE/bakki6.gif";
$BGBAK3 = "$IMAGES_BASE/bg26.png";
$TABCOL = "40E0FF";
#
$PRIM_ADMIN_EMAIL = "seuranta\@vunet.org";

#
#if(NoTracking())
#{
#$dbh = DBI->connect("DBI:mysql:database=vunet;host=127.0.0.1;", "root", "vf3890X", {RaiseError => 1});
#}

#####################################################################################################
#
sub DetermineURLIconImage
{
	my ($str);

	#
	$str = $_[1];
	$str =~ s/^http:\/\/[a-z]+\.vunet\.org[^\/]*[\/]//;
	$str =~ s/([a-z]+)\/[^0-9]+([0-9]+).*$/$1\/pub_artikkeli$2.txt/;
	#
	if($str=~/pub_artikkeli/)
	{
		#
		$so{'imageurl'} = "";
		if(-e "$str\_options.txt")
		{
			LoadImageURL("$str\_options.txt");
		}
		if($so{'imageurl'} eq "") { goto past; }
		$so{'imageurl'} =~ s/\.jpg$/\.png/;
		$so{'imageurl'} =~ s/^.*\/([^\/]+)$/$1/;
		$so{'imageurl'} = "$IMAGES_BASE/thumb2/th_$so{'imageurl'}";
		return $so{'imageurl'};
	}
	else
	{
past:
		#
		return "http://www.vuutiset.info/images/red-ball.gif";
	}

	#
}

#####################################################################################################
#
# GetArticleTitles [section, f.e. ulkomaat] [max. count]
# Returns an array: [title] [id]
#
sub GetArticleTitles
{
	my (@tmp,@art,@art2,$section,$id,$q,$i,$i2);

	#
	print STDERR "$_[0], $_[1]\n";

	#
	$q = $_[0];
	$q =~ s/^\.\.\///g;
	$q =~ s/^\.\///g;
	$section = $q;
	$section =~ s/^([a-z]+).*$/$1/g;
	$id = $q;
	$id =~ s/[^0-9]//g;

	#
	if($_[1]<=0)
	{
		$lim = "100";
	}
	else
	{
		$lim = $_[1];
	}
	my $query = "SELECT title,section,artid FROM ArticleDetails section=$section ORDER BY date DESC LIMIT 0,$lim";
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $i ($DBI::errstr)\n";
#	if(NoTracking()) { print "<LI>$query</LI><BR>"; }

        #
	@tmp = $sth->fetchrow_array();

	#
	return @tmp;
}

#####################################################################################################
#
# GetArticleTitles
# Returns an array: [title] [section] [id]
#
sub GetLatestArticles
{
	my (@lst,@row,@art,@art2,$section,$id,$q,$i,$i2,$sth,$query);

	#
	#######my $query = "SELECT * FROM visitors$WHERE ORDER BY TIME DESC LIMIT 0, $so{'depth'}";
	$query = "SELECT title,section,artid FROM ArticleDetails ORDER BY id DESC LIMIT 0, 100;";
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $query ($DBI::errstr)\n";

	#
	@lst = ();

	#
	while(@row = $sth->fetchrow_array())
	{
		push(@lst,@row);
	}

	#
	return @lst;
}

#####################################################################################################
#
# GetArticleTitleFDB [file - f.e. ulkomaat/pub_artikkeli10.txt]
#
sub GetArticleTitleFDB
{
	my (@lst);

	#
	@lst = LoadList($_[0]);
	$lst[0] =~ s/<[^\>]+>//gi;
	$lst[0] =~ s/^(\s+)//g;
	$lst[0] =~ s/(\s+)$//g;
	return $lst[0];
}

#####################################################################################################
#
# GetArticleTitle [file - f.e. ulkomaat/pub_artikkeli10.txt]
# Returns an array: [title] [date]
#
sub GetArticleTitle
{
	my (@tmp,@art,@art2,$section,$id,$q,$i,$i2);

	#
	#print STDERR "$_[0]\n";

	#
	$q = $_[0];
	$q =~ s/^\.\.\///g;
	$q =~ s/^\.\///g;
	$section = $q;
	$section =~ s/^([a-z]+).*$/$1/g;
	$id = $q;
	$id =~ s/[^0-9]//g;

        #
	if($_[1]<=0)
	{
		$lim = "100";
	}
	else
	{
		$lim = $_[1];
	}
	my $query = "SELECT title,date FROM ArticleDetails WHERE artid=$id AND section='$section' ORDER BY date DESC LIMIT 0,$lim";
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $i ($DBI::errstr)\n";
#	if(NoTracking()) { print "<LI>$query</LI><BR>"; }

        #
	@tmp = $sth->fetchrow_array();

	#
	$tmp[0] =~ s/<[^\>]+>//gi;

	#
	return @tmp;
}

#####################################################################################################
#
# SetArticleTitle ulkomaat/pub_artikkeli10.txt title [date]
# Parameters: file title date
#
sub SetArticleTitle
{
	my (@tmp,@art,@art2,$section,$id,$q,$i,$i2,$pvm);

	#
	$pvm = $_[2];
	#print STDERR "$_[0] = '$_[1]', date = $pvm\n";

	#
	$q = $_[0];
	$q =~ s/^\.\.\///g;
	$q =~ s/^\.\///g;
	$section = $q;
	$section =~ s/^([a-z]+).*$/$1/g;
	$id = $q;
	$id =~ s/[^0-9]//g;

	#
	my $query = ("DELETE FROM ArticleDetails WHERE artid=$id AND section='$section';");
	#print "$query\n";
	$sth = $dbh->prepare($query);
	$sth->execute() or print "#1 failed on $i ($DBI::errstr)\n";

        #
	my $query = ("INSERT INTO ArticleDetails
(artid,section,title,date)
values
('$id','$section','$_[1]','$pvm');
");
	#print "$query\n";
	$sth = $dbh->prepare($query);
	$sth->execute() or print "#2 failed on $i ($DBI::errstr)\n";

	#
}

#####################################################################################################
#
# GetArticle [file name]
# GetArticle ulkomaat/pub_artikkeli10.txt
#
sub GetArticle
{
	my (@tmp,@art,@art2,$section,$id,$q,$i,$i2);

	#
	return LoadList($_[0]);

	#

	#
	$q = $_[0];
	$q =~ s/^\.\.\///g;
	$q =~ s/^\.\///g;
	$section = $q;
	$section =~ s/^([a-z]+).*$/$1/g;
	$id = $q;
	$id =~ s/[^0-9]//g;

        #
	my $query = "SELECT title,body FROM body_$section WHERE id=$id";
	#if(NoTracking()) { print "<LI>$query</LI><BR>"; }
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $i ($DBI::errstr)\n";

        #
	@tmp = $sth->fetchrow_array();

	#
	@art = ();
	$tmp[1] =~ s/<br>/<br>/gi;
	$tmp[1] =~ s/<br>/<br>\n/gi;
	@art = split(/<br>/, $tmp[1]);
	for($i=0,$i2=0; $i<($#art+1); $i++)
	{
		$art[$i] = "$art[$i]<br>\n";
	}
	push(@art2, $tmp[0]);
	push(@art2, @art);

	#
	return @art2;
}

#####################################################################################################
#
# StoreArticle ulkomaat/pub_artikkeli10.txt
#
sub StoreArticle
{
	my (@tmp,@art,@art2,$section,$id,$q,$i,$i2);

	#
	$q = $_[0];
	$q =~ s/^\.\.\///g;
	$q =~ s/^\.\///g;
	$section = $q;
	$section =~ s/^([a-z]+).*$/$1/g;
	$id = $q;
	$id =~ s/[^0-9]//g;

        #
	my $query = "DELETE FROM body_$section WHERE id=$id";
	if(NoTracking()) { print "<LI>$query</LI><BR>"; }
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $i ($DBI::errstr)\n";

        #
	my $query = ("INSERT INTO FROM body_$section
(id,section,title,body)
('$id','$section','$ARTICLE[0]','$ARTICLE[1]')
");
	if(NoTracking()) { print "<LI>$query</LI><BR>"; }
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $i ($DBI::errstr)\n";

        #
	@tmp = $sth->fetchrow_array();

	#
	@art = ();
	$tmp[1] =~ s/<br>/<br>/gi;
	$tmp[1] =~ s/<br>/<br>\n/gi;
	@art = split(/<br>/, $tmp[1]);
	for($i=0; $i<($#art+1); $i++) { $art[$i] = "$art[$i]<BR>"; }
	push(@art2, $tmp[0]);
	push(@art2, @art);

	#
	return @art2;
}

#####################################################################################################
#
sub GetArticlesOptions
{
	my (@lst);

	#
	LoadVars("$_[0]\_options.txt");
	$so{'imageurl'} =~ s/vuutiset.info/vunet.org/g;
}

#####################################################################################################
#
sub GetArticleIPs
{
	my (@lst,$str);

	#
	$str = $_[0];
	$str =~ s/\.txt$/\.counter/;
	@lst = LoadList("$str");
	return @lst;
}

#####################################################################################################
#
sub GetArticleReferences
{
	my (@lst,$str);

	#
	$str = $_[0];
	@lst = LoadList("$str\_ref.txt");
	return @lst;
}

#####################################################################################################
#
sub GetArticleIPs
{
	my (@lst,$str);

	#
	$str = $_[0];
	@lst = LoadList("$str\_thumbs.txt");
	return @lst;
}

#####################################################################################################
#
sub vis_log
{
        my ($f,$f2,$str,$str2,$t,$pm);

        #
        $t = time;

        #
        $pm = sprintf("%d", $t/86400);

        #
        open($f, ">>articles/logs/vislog/vislog-$pm.txt");
        flock $f, LOCK_EX;
        print $f "$t;$ENV{'REMOTE_ADDR'};$ENV{'REMOTE_HOST'};\n";
        flock $f, LOCK_UN;
        close($f);
}

#############################################################################
#
sub save_iopts
{
        my ($i,$i2,$str,$str2,$f,$fn);

        #
        $fn = "$_[0]\_options.txt";
        if(-e $fn)
        {
                # Get existing variables which aren't changed now.
                LoadNewVars($fn);
        }
        open($f, ">$fn") || die "can't open $fn for writing";
        for($i=0; $i<($#iopt_keys+1); $i++)
        {
                #
		if($iopt_keys[$i]=~/imageurl/)
		{
			# Accept only jpegs as imageurls.
			if( !($so{$iopt_keys[$i]}=~/\.jpg/) )
			{
				$so{$iopt_keys[$i]} = "";
			}
		}
                $so{$iopt_keys[$i]} =~ s/\n/<BR>/g;
                print $f "$iopt_keys[$i]=$so{$iopt_keys[$i]}\n";
        }
	print $f "lyrics=$so{'lyrics'}\n";
	print $f "stream=$so{'stream'}\n";
	print $f "mirror=$so{'mirror'}\n";
	print $f "commir=$so{'commir'}\n";
	print $f "release_date=$so{'release_date'}\n";
        close($f);
}

######################################################################################################
#
sub isAutoSection
{
	my ($i,$i2,$f,@ql,$quoteit,$tep,$author,$cap,@art,$x);
	my @asecs=(
        "ostetaan",
        "myydaan",
        "kannettavat",
        "valokuvaus",
        "asunnot",
        "tyopaikat",
        "tyonhakijat",
        "komponentit",
        "elokuvat",
        "veneily",
        "huumori",
        "ruokailu",
	"progressive",
	"bush",
	);

        #
        for($i=0; $i<($#asecs+1); $i++)
        {
                if($asecs[$i] eq $_[0]) { return 1; }
        }

        #
        return 0;
}

######################################################################################################
#
sub isEnglishSection
{
	my ($i,$i2,$f,@ql,$quoteit,$tep,$author,$cap,@art,$x);
	my @esecs=(
	"progressive",
	"bush",
	"picks",
	"videos"
	);

        #
        for($i=0; $i<($#esecs+1); $i++)
        {
                if($esecs[$i] eq $_[0]) { return 1; }
        }

        #
        return 0;
}

######################################################################################################
#
sub isSwedishSection
{
	my ($i,$i2,$f,@ql,$quoteit,$tep,$author,$cap,@art,$x);
	my @esecs=(
	"nyheter",
	);

        #
        for($i=0; $i<($#esecs+1); $i++)
        {
                if($esecs[$i] eq $_[0]) { return 1; }
        }

        #
        return 0;
}

######################################################################################################
#
sub isDutchSection
{
	my ($i,$i2,$f,@ql,$quoteit,$tep,$author,$cap,@art,$x);
	my @esecs=(
	"dutch",
	);

        #
        for($i=0; $i<($#esecs+1); $i++)
        {
                if($esecs[$i] eq $_[0]) { return 1; }
        }

        #
        return 0;
}

######################################################################################################
#
# CapUrl [URL CAP].
#
sub CapUrl
{
	my ($url,$urlcap);

	#
	$urlcap = $_[1];
	$urlcap =~ tr/[A-ZÄÖÅ]/[a-zäöå]/;
	$urlcap =~ s/ä/a/g;
	$urlcap =~ s/ö/o/g;
	$urlcap =~ s/å/o/g;
        $urlcap =~ s/[^a-z0-9]/_/g;
        $urlcap =~ s/_*$//g;
        $urlcap =~ s/^_*//g;
        $urlcap =~ s/__/_/g;
        $urlcap =~ s/^(.{70}[a-z0-9]*).*$/$1/;
        $url = $_[0];
	$url =~ s/pub_artikkeli([0-9]*).txt/story$1.html/;
        $url =~ s/\/english\//\/picks\//;
        $url =~ s/\/article//;
        $url =~ s/story([0-9]*)/$urlcap-$1/;
	#print "$url<BR>";
        return $url;
}

################################################################
#
sub GenName
{
	my ($num,$tm);

	#
	$num = rand(100000000000000);
	$num =~ s/\.//;
	$tm = time;
	return "_$tm$num$num2.tmp";
}

#
sub CookieTools
{
	print("
<SCRIPT LANGUAGE=\"JavaScript\">
<!--

// name - name of the cookie
// value - value of the cookie
// [expires] - expiration date of the cookie (defaults to end of current session)
// [path] - path for which the cookie is valid (defaults to path of calling document)
// [domain] - domain for which the cookie is valid (defaults to domain of calling document)
// [secure] - Boolean value indicating if the cookie transmission requires a secure transmission
// * an argument defaults when it is assigned null as a placeholder
// * a null placeholder is not required for trailing omitted arguments
function setCookie(name, value, expires, path, domain, secure) {
  var curCookie = name + \"=\" + escape(value) +
      ((expires) ? \"; expires=\" + expires : \"\") +
      ((path) ? \"; path=\" + path : \"\") +
      ((domain) ? \"; domain=\" + domain : \"\") +
      ((secure) ? \"; secure\" : \"\");
  document.cookie = curCookie;
}

// name - name of the desired cookie
// * return string containing value of specified cookie or null if cookie does not exist
function getCookie(name) {
  var dc = document.cookie;
  var prefix = name + \"=\";
  var begin = dc.indexOf(\"; \" + prefix);
  if (begin == -1) {
    begin = dc.indexOf(prefix);
    if (begin != 0) return null;
  } else
    begin += 2;
  var end = document.cookie.indexOf(\";\", begin);
  if (end == -1)
    end = dc.length;
  return unescape(dc.substring(begin + prefix.length, end));
}

// name - name of the cookie
// [path] - path of the cookie (must be same as path used to create cookie)
// [domain] - domain of the cookie (must be same as domain used to create cookie)
// * path and domain default if assigned null or omitted if no explicit argument proceeds
function deleteCookie(name, path, domain) {
  if (getCookie(name)) {
    document.cookie = name + \"=\" + 
    ((path) ? \"; path=\" + path : \"\") +
    ((domain) ? \"; domain=\" + domain : \"\") +
    \"; expires=Thu, 01-Jan-70 00:00:01 GMT\";
  }
}

// date - any instance of the Date object
// * hand all instances of the Date object to this function for \"repairs\"
function fixDate(date) {
  var base = new Date(0);
  var skew = base.getTime();
  if (skew > 0)
    date.setTime(date.getTime() - skew);
}

// -->
</SCRIPT>

	");
}


#
sub gotUserFile
{
	my ($f);

        #
        open($f, "$NWPUB_CGIBASE/user/user_$_[0].txt") || return 0;
        $userid = <$f>;
        $firstname = <$f>;
        $lastname = <$f>;
        $email = <$f>;
        $profession = <$f>;
        close($f);
        return 1;
}

######################################################################
# Load a list into array.
#
sub LoadList
{
        my ($_f,@_ulst,$ich,$ich2,$str);

        #
        if( !open($_f, $_[0]) )
	{
		#$str = "Can't open $_[0]";
	       # $str =~ s/(.{8})/$1 /g;
		#print("<fontsize=2 color=\"#000000\">$str</font>\n");
		return;
	}
	flock $_f, LOCK_EX;
        @_ulst = <$_f>;
	flock $_f, LOCK_UN;
        close($_f);

        #
        for($ich=0; $ich<($#_ulst+1); $ich++)
        {
                chomp $_ulst[$ich];
		$_ulst[$ich] =~ s/\s*$//g;
		$_ulst[$ich] =~ s/^\s*//g;
        }

        #
        return @_ulst;
}

# PostMessage file1.txt file2.txt address@host.com subject_here
sub MailMessage
{
	my ($viesti);

	#
	$viesti = $_[3];
	$viesti =~ s/<br>//i;
	$viesti =~ s/<br>//i;
	$viesti =~ s/<hr>//i;
	system "cat $_[0] $_[1] | mail $_[2] Subject: $viesti";
}

# Write a string to the administration log.
sub AdminLog
{
	#
	system "echo \"$_[0]\" >> ./admin-log.txt";
}

#
sub OpenWebIndex
{
	my ($f,$i);

        #
	@web = LoadList($_[0]);
	#
	$wherebe = 0;

	#
	return @web;
}

##########################################################
sub HandleRest
{
	my ($ix);

        # Print until end of the web text.
        for($ix=$wherebe; $ix<($#web+1); $ix++)
        {
                print "$web[$ix]\r\n";
        }
#	print ">\n";
}

##########################################################
sub OpenIndex
{
        my ($i,$ff);

	#
	@fileindex = LoadList($_[0]);
}


##########################################################
#
# Handles an external section.
#
# [section trigger], [perl program to make the output]
#
sub HandleExternal
{
        my ($i,$i2,$found);

        # Print until "$_[0]" is found.
        loop1: for($i=$wherebe,$found=0; $i<($#web+1) && !$found; $i++)
        {
                if($web[$i] =~ /$_[0]/i)
                {
                        $found=1;
                }
                print "$web[$i]\r\n";
        }
        if($found) { system $_[1]; }
        $wherebe=$i;
}

#
sub WebWalkTo
{
        my ($i,$i2,$loytyi);

        # Print until "$_[0]" is found.
        loop1: for($i=$wherebe,$loytyi=0; $i<($#web+1) && !$loytyi; $i++)
        {
                if($web[$i] =~ /$_[0]/i)
                {
                        $loytyi=1;
                }
                print "$web[$i]\r\n";
        }
	if(!$loytyi)
	{
		#print "<div>\"$_[0]\" NOT FOUND, \#web = $#web, i=$i </div>";
	}
        $wherebe=$i;
}

#
sub SkipTo
{
        my ($i,$i2,$found);

        # Print until "$_[0]" is found.
        loop1: for($i=$wherebe,$found=0; $i<($#web+1) && !$found; $i++)
        {
                if($web[$i] =~ /$_[0]/i)
                {
                        $found=1;
                }
   #             print "$web[$i]\r\n";
        }
        $wherebe=$i;
}

#
sub WebModif
{
	my ($z);

	#
	for($z=0; $z<($#web+1); $z++)
	{
	        $web[$z] =~ $_[0];
	}
}

#
sub Corner
{
        #
        print("
                <table width=\"100%\">
                <tr>
                        <td>
                ");

        #
        print("

                <table width=\"100%\" vspace=\"8\" hspace=\"32\" align=\"center\">
                <tr><td>

		<center>
                <form method=\"get\" action=\"search.pl\" name=\"search\">
                <input type=\"text\" name=\"searchstring\" size=\"8\" value=\"\">
                <input type=\"hidden\" name=\"cmd\" value=\"searchnow\">
                <option selected>
                <input type=\"submit\" value=\"Etsi\" style=\"background-color: #FFC0C0\">
                </form>
		</center>

<SCRIPT LANGUAGE=\"JavaScript\">
document.forms[0].elements[0].focus();
</script>


                </td></tr>
                </table>
                ");

        #
        print("
                        </td>
                </tr>
                </table>
                ");
}

#
sub JSTools
{
        #
print("
<SCRIPT LANGUAGE=\"JavaScript\">
<!--

// name - name of the cookie
// value - value of the cookie
// [expires] - expiration date of the cookie (defaults to end of current session)
// [path] - path for which the cookie is valid (defaults to path of calling document)
// [domain] - domain for which the cookie is valid (defaults to domain of calling document)
// [secure] - Boolean value indicating if the cookie transmission requires a secure transmission
// * an argument defaults when it is assigned null as a placeholder
// * a null placeholder is not required for trailing omitted arguments
function setCookie(name, value, expires, path, domain, secure) {
  var curCookie = name + \"=\" + escape(value) +
      ((expires) ? \"; expires=\" + expires.toGMTString() : \"\") +
      ((path) ? \"; path=\" + path : \"\") +
      ((domain) ? \"; domain=\" + domain : \"\") +
      ((secure) ? \"; secure\" : \"\");
  document.cookie = curCookie;
}

// name - name of the desired cookie
// * return string containing value of specified cookie or null if cookie does not exist
function getCookie(name) {
  var dc = document.cookie;
  var prefix = name + \"=\";
  var begin = dc.indexOf(\"; \" + prefix);
  if (begin == -1) {
    begin = dc.indexOf(prefix);
    if (begin != 0) return null;
  } else
    begin += 2;
  var end = document.cookie.indexOf(\";\", begin);
  if (end == -1)
    end = dc.length;
  return unescape(dc.substring(begin + prefix.length, end));
}

// name - name of the cookie
// [path] - path of the cookie (must be same as path used to create cookie)
// [domain] - domain of the cookie (must be same as domain used to create cookie)
// * path and domain default if assigned null or omitted if no explicit argument proceeds
function deleteCookie(name, path, domain) {
  if (getCookie(name)) {
    document.cookie = name + \"=\" +
    ((path) ? \"; path=\" + path : \"\") +
    ((domain) ? \"; domain=\" + domain : \"\") +
    \"; expires=Thu, 01-Jan-70 00:00:01 GMT\";
  }
}

// date - any instance of the Date object
// * hand all instances of the Date object to this function for \"repairs\"
function fixDate(date) {
  var base = new Date(0);
  var skew = base.getTime();
  if (skew > 0)
    date.setTime(date.getTime() - skew);
}

// -->
</SCRIPT>
        ");

        #
}

#
sub GetCookies
{
	my (@cookies,$_cl);

	#
	$_cl = $ENV{'HTTP_COOKIE'};
	#
	@cookies = split /;/, $_cl;

	#
	return @cookies;
}

#
sub GetCookie1
{
	my ($i,@cookies);

	#
	@cookies = GetCookies();

        #
        for($i=0; $i<($#cookies+1); $i++)
        {
                #
                if($cookies[$i] =~ /$_[0]\=/)
                {
                        return $cookies[$i];
                }
        }

        #
        return "";
}

#
sub GetCookie
{
	my ($tmp);

	#
	$tmp = GetCookie1($_[0]);
	$tmp =~ s/$_[0]\=//g;
	return $tmp;
}

#
sub GetUserID
{
	my ($usrid);

	#
	$usrid = GetCookie("userid");

	#
	$usrid =~ s/\D//g;
	return $usrid;
}

#
%so =   (
	);

# Save user profile.
sub SaveProfile
{
	my ($key);

        #
        $so{'lastaccessdate'} = time;

        #
#        open(f, ">$VAI_PROF_DIR$_[0]\.profile") || die "error while creating user profile, please contact vaihtoehtouutiset\@sci.fi";
        #
#        foreach $key (keys %so)
#        {
#            print f "$key=$so{$key}\n";
#        }
#        close(f);
}

# Load variables to %so tree.
sub LoadVars
{
        my ($i,$str,$str2,$str3,$str4,@tmp,$f,@opt);

        #
	if( !open($f, "$_[0]") )
	{
		#print "NOT FOUND $_[0]\n";
		return;
	}
        close($f);
        @opt = LoadList("$_[0]");

        #
        for($i=0; $i<($#opt+1); $i++)
        {
                $str = $opt[$i];
                @tmp = split("\=", $str);
                $varname = $tmp[0];
                $varvalue = $tmp[1];
                if($varname ne "" && $varvalue ne "")
                {
                        $so{$varname} = $varvalue;
                }
        }

	#
        return 1;
}

# Load variables to %so tree.
sub LoadImageURL
{
        my ($i,$str,$str2,$str3,$str4,@tmp,$f,@opt);

        #
	if( !open($f, "$_[0]") )
	{
		print "NOT FOUND $_[0]\n";
		return;
	}
        close($f);
        @opt = LoadList("$_[0]");

        #
        for($i=0; $i<($#opt+1); $i++)
        {
                $str = $opt[$i];
                @tmp = split("\=", $str);
                $varname = $tmp[0];
                $varvalue = $tmp[1];
                if($varname eq "imageurl" && $varvalue ne "")
                {
                        $so{$varname} = $varvalue;
                }
        }

	#
        return 1;
}

# Load variables to %so tree.
sub LoadNewVars
{
        my ($i,$str,$str2,$str3,$str4,@tmp,$f,@opt);

        #
	if( !open($f, "$_[0]") )
	{
		print "NOT FOUND $_[0]\n";
		return;
	}
        close($f);
        @opt = LoadList("$_[0]");

        #
        for($i=0; $i<($#opt+1); $i++)
        {
                $str = $opt[$i];
                @tmp = split("\=", $str);
                $varname = $tmp[0];
                $varvalue = $tmp[1];
                if($varname ne "" && $varvalue ne "")
                {
                        if($so{$varname} eq "") { $so{$varname} = $varvalue; }
                }
        }

	#
        return 1;
}

# Load user profile.
sub LoadProfile
{
        my ($i,$str,$str2,$str3,$str4,@tmp);

        #
        open(f, "$VAI_PROF_DIR$_[0]\.profile") || return 0;
        close(f);
        @opt = LoadList("$VAI_PROF_DIR$_[0]\.profile");

	#
	%so = "";

        #
        for($i=0; $i<($#opt+1); $i++)
        {
                $str = $opt[$i];
		VarSet($str);
        }
        return 1;
}

# Load configuration.
sub LoadConfiguration
{
	my ($u);

	#
	$u = GetUserID();
	if( !LoadProfile($u) )
	{
		#
		SaveProfile($u);
	}

	#
	$ENV{'CFG_LOADED'} = "TRUE";
}

#
sub NoTracking
{
	#
	if(GetCookie("vunetadmin") eq "true") { return "1"; }

	#
	if($ENV{'REMOTE_HOST'} =~ /cache/i || $ENV{'REMOTE_HOST'} =~ /proxy/i)
	{
		return "0";
	}

	#
	if($ENV{'NO_TRACKING'} ne "") { return $ENV{'NO_TRACKING'}; }

	#
	if(_NoTracking())
	{
		$ENV{'NO_TRACKING'} = "1";
	}
	else
	{
		$ENV{'NO_TRACKING'} = "0";
	}
	return $ENV{'NO_TRACKING'};
}

#
sub _NoTracking
{
        my ($str);
        my ($i,@igl);

	#
	for($i=0; $i<($#MASTER_IPS+1); $i++)
	{
		if( $ENV{'REMOTE_ADDR'} eq $MASTER_IPS[$i] )
		{
			return 1;
		}
	}

	#
#	if( $ENV{'REMOTE_ADDR'} eq $MASTER_IP )
#	{
#		return 1;
#	}
#
        #
        @igl = LoadList("$ENV{'DOCUMENT_ROOT'}/articles/cfg/notracking.txt");

        #
        for($i=0; $i<($#igl+1); $i++)
        {
                if($igl[$i] eq $ENV{'REMOTE_ADDR'})
                {
                        # This userid is being ignored.
                        return 1;
                }
        }
	return 0;
}

#
sub PathToSection
{
	my (@s);

        #
        @s = split "\/", $_[0];
        return $s[$#s-1];
}

# Types in a html code of 1 pixel height line by using a table.
sub Line
{
        print("
                        <table width=\"$_[1]\"
                        cellpadding=\"0\" cellspacing=\"0\">
                        <tr>
                        <td width=\"100%\" bgcolor=\"$_[0]\" height=\"1\">
                        </td>
                        </tr>
                        </table>
                ");
}

# SelectionBox
# Requires four parameters:
# AC, AC2, String To Show, URL To Link, table alignment (alignment is optional)
sub SelectionBox
{
                #
                Line("$_[0]", "120");
                print("
                        <table
                        cellpadding=\"0\" cellspacing=\"0\" align=\"$_[4]\">


                        <tr class=\"withLink\" bgcolor=\"$_[1]\">
                        <td width=\"16\" bgcolor=\"$_[0]\">
                        </td>

                        <td width=\"4\">
                        </td>

                        <td width=\"100\">
                        <a href=\"$_[3]\" style=\"color: rgb(255, 255, 255);\">
                        <font size=\"1\">
                        $_[2]
                        </font>
                        </a>
                        </td>

                        </tr>


                        </table>

                        ");

                #
                Line("", 120);
}

# Hunt for most recently updated file.
sub huntn
{
	my ($i,$i2,$i3,$i4,$fname,@li,$f,$ff);
	
	#
#	@li = LoadList("./$_[0]/fileindex.txt");

	#
	open($f, "./$_[0]/fileindex.txt") || die "can't open important-sections.txt";;
	@li = <$f>;
	for($i=0; $i<($#li+1); $i++) { chomp $li[$i]; }
	close($f);

	#
	return "./$_[0]/$li[$#liz]";
}

##########################################################
sub BigText
{
        my ($str,$str2,$i,$i2);

        #
        $str = $_[0];
        $str =~ tr/[a-zï¿½ï¿½ï¿½]/[A-Zï¿½ï¿½ï¿½]/;

        #
        print("
                <font color=\"#C00000\" size=\"6\" face=\"Arial\">
		<b>$str</b>
                </font>
                ");
}

###########################################################################################
#
sub Seperator
{
        #
        print("
                <table bgcolor=\"0\" width=640 height=4
                        cellspacing=\"0\"
                        cellpadding=\"0\">
                <tr valign=top>
                        <td bgcolor=\"#002000\">
                        </td>
                </tr>
                </table>
                ");
}

###########################################################################################
#
sub SectionHeadline
{
	#
	if($_[0] eq "")
	{
	        #
        	BigText($ENV{'CURSEC'});

		#
		Seperator();
	}
	else
	{
	        #
        	BigText($_[0]);

		#
		Seperator();
	}
}

###########################################################################################
#
sub DetectLanguage
{
        my ($i,$i2,$i3,$i4,$tmp,$tmp2,$str,$str2,@l,$co,@s,@s2,$change,$r,@sp);

	#
	$r = $ENV{'REMOTE_HOST'};
	if($r=~/\.cn$/)
	{
		return "cn";
	}
	if($r=~/\.nl$/)
	{
		return "nl";
	}
	if($r=~/\.se$/)
	{
		return "se";
	}
	if($r=~/\.fi$/)
	{
		return "fi";
	}

	#
	@sp = split(/[\,\;]/, $ENV{'HTTP_ACCEPT_LANGUAGE'});
	for($i=0; $i<($#sp+1); $i++)
	{
		if($sp[$i] eq "nl") { return "nl"; }
		if($sp[$i] eq "cn") { return "cn"; }
		if($sp[$i] eq "fi") { return "fi"; }
		if($sp[$i] eq "se") { return "se"; }
		if($sp[$i] eq "en") { return "en"; }
	}

	return "en";
}

###########################################################################################
#
sub ArgLineParse
{
        my ($i,$i2,$i3,$i4,$tmp,$tmp2,$str,$str2,@l,$co,@s,@s2,$change,$r,$l,@sp,@cookies,$p);

	#
#	$CGI_QUERY = new CGI;

	#
	$ENV{'NW_LANGUAGE'} = DetectLanguage();

        #
	$p = $ENV{'HTTP_REFERER'};
	$p =~ s/^http:\/\/www\.vunet\.org//;
	if($ENV{'HTTP_REFERER'}=~/www\.vunet\.org/ ||
	$ENV{'HTTP_REFERER'}=~/vuutiset\.info/)
	{
	        if($p=~/^\/uutiset/) { $ENV{'NW_LANGUAGE'} = "fi"; }
	        if($p=~/^\/xinwen/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/nieuws/) { $ENV{'NW_LANGUAGE'} = "nl"; }
	        if($p=~/^\/nyheter/) { $ENV{'NW_LANGUAGE'} = "se"; }
	        if($p=~/^\/swedish/) { $ENV{'NW_LANGUAGE'} = "se"; }
	        if($p=~/^\/favorites/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/404\.pl$/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/news/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/progressive/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/bush/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/picks/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/english/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/videos/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/downloads/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/section\=videos/) { $ENV{'NW_LANGUAGE'} = "en"; }
	        if($p=~/^\/TopTen\.pl/) { $ENV{'NW_LANGUAGE'} = "en"; }
	}

	#
	if($ENV{'NW_LANGUAGE'} eq "en") { $so{'FP_SECTION'} = "english"; }

        # Read in text.
        $ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;
        if ($ENV{'REQUEST_METHOD'} eq "POST")
        {
		read(STDIN, $tmp, $ENV{'CONTENT_LENGTH'});
        }
        else
        {
	        # Get section argument.
	        $tmp = $ENV{'QUERY_STRING'};
        }

	#
        $tmp =~ s/\+/ /ig;
	$tmp =~ s/%3D/!FCE2E37B!/gi;
	$tmp =~ s/%26/!FF325136!/gi;
        $tmp =~ s/%(..)/pack("C", hex($1))/eg;

	#
        @s = split("\&", $tmp);

        # Section always defaults to none
        # (always show front page if no section= specified).
        $so{'section'} = "";
	# No redirection as default.
	$so{'redirect'} = "";

        #
        for($i=0,$change=0; $i<($#s+1); $i++)
        {
                #
                if($s[$i] =~ /\=/)
                {
                        #
                        $change = 1;

                        #
                        @s2 = split("\=", $s[$i]);

                        #
                        $so{$s2[0]} = $s2[1];
			$so{$s2[0]} =~ s/!FCE2E37B!/\=/g;
			$so{$s2[0]} =~ s/!FF325136!/\&/g;
                }
        }

        #
        if($change && !$DONT_AFFECT_DB) { SaveProfile(GetUserID()); }

	#
	$l = $so{'section'};
        if($l eq "english" || $l eq "videos" || $l eq "picks"
                || $l eq "commodore"
                || $l eq "software"
                || $l eq "progressive"
                || $l eq "global"
                || $l eq "bush")
	{
		$ENV{'NW_LANGUAGE'} = "en";
		if($so{'FP_SECTION'} eq "") { $so{'FP_SECTION'} = "english"; }
	}
        if($l eq "nyheter")
	{
		$ENV{'NW_LANGUAGE'} = "se";
		if($so{'FP_SECTION'} eq "") { $so{'FP_SECTION'} = "swedish"; }
	}

	# Fetch cookies.
	@cookies = split(/\;/, $ENV{'HTTP_COOKIE'});
	for($i=0; $i<($#cookies+1); $i++)
	{
		$str = $cookies[$i];
		@sp = split(/\=/, $str);
		$str = "\"cookie\_$sp[0]";
		$str =~ s/[^a-z0-9_]//g;
		$sp[1] =~ s/%(..)/pack("C", hex($1))/eg;
		$so{$str} = $sp[1];
	}

	#
	$ENV{'NW_LOGO_LANGUAGE'} = $ENV{'NW_LANGUAGE'};

	#
#	if($so{'cookie_language'} eq "English")    { $ENV{'NW_LANGUAGE'} = "en"; }
#	if($so{'cookie_language'} eq "Nederlands") { $ENV{'NW_LANGUAGE'} = "nl"; }
#	if($so{'cookie_language'} eq "Svenska")    { $ENV{'NW_LANGUAGE'} = "se"; }
#	if($so{'cookie_language'} eq "Suomi")      { $ENV{'NW_LANGUAGE'} = "fi"; }

        #
	return $ENV{'REQUEST_METHOD'};
}

#
sub GoogleAds_2
{
	#
	print("
<table align=\"right\">
<tr>
<td>

<script type=\"text/javascript\"><!--
google_ad_client = \"pub-4178289363390566\";
google_ad_width = 125;
google_ad_height = 125;
google_ad_format = \"125x125_as\";
google_ad_channel =\"\";
google_ad_type = \"text\";
google_color_border = \"336699\";
google_color_bg = \"FFFFFF\";
google_color_link = \"0000FF\";
google_color_url = \"008000\";
google_color_text = \"000000\";
//--></script>
<script type=\"text/javascript\"
  src=\"http://pagead2.googlesyndication.com/pagead/show_ads.js\">
</script>

</td>
</tr>
<table>
	");
}

#
sub GoogleAds_3
{
	#
	print("
<table align=\"center\">
<tr>
<td>


<div class=\"supah\">

<script type=\"text/javascript\"><!--
google_ad_client = \"pub-4178289363390566\";
google_ad_width = 300;
google_ad_height = 250;
google_ad_format = \"300x250_as\";
google_ad_channel =\"\";
google_ad_type = \"text_image\";
google_color_border = \"E0FFE3\";
google_color_bg = \"E0FFE3\";
google_color_link = \"0000CC\";
google_color_url = \"008000\";
google_color_text = \"000000\";
//--></script>
<script type=\"text/javascript\"
  src=\"http://pagead2.googlesyndication.com/pagead/show_ads.js\">
</script>

</div>

</td>
</tr>
<table>
	");
}

#
sub detout
{
	if($_[0] eq "")
	{
		return stdout;
	}
	else
	{
		return $_[0];
	}
}

#
sub GoogleAds
{
        #
	printf("
<table align=\"right\">
<tr>
<td>

<br>
<br>
<br>
<br>

<br>
<br>
<br>
<br>

<br>
<br>
<br>
<br>

<br>
<br>
<br>
<br>

<script type=\"text/javascript\"><!--
google_ad_client = \"pub-4178289363390566\";
google_ad_width = 120;
google_ad_height = 600;
google_ad_format = \"120x600_as\";
google_ad_channel =\"\";
google_color_border = \"336699\";
google_color_bg = \"FFFFFF\";
google_color_link = \"0000FF\";
google_color_url = \"008000\";
google_color_text = \"000000\";
//--></script>
<script type=\"text/javascript\"
  src=\"http://pagead2.googlesyndication.com/pagead/show_ads.js\">
</script>

</td>
</tr>
<table>
	");
}

#
sub CustomTimeString
{
	my ($Second,$Minute,$Hour,$Day,$Month,
		$Year,$WeekDay,$DayOfYear,$IsDST,
		$Year1,$Month1,$Day1,
		$str,$str2);

	#
	($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime(time);

	#
	$Year1 = $Year + 1900;
	$Month1 = $Month+1;

	#
	$str = "$Hour:$Minute, $Day.$Month1.$Year1";
	return $str;
}

#
sub WebStart
{
	#
	OpenWebIndex("./webindex.html");
	#
	WebWalkTo("past-uusilogo2");
	if($_[0] ne "")
	{
		print("
			<img src=\"$_[0]\" alt=\"$_[0]\" border=0>
			");
	}
	#
	HandleExternal("main-menu", "./mainmenu.pl");
}

#
sub EndBar
{
	my ($i,$i2,$i3,$i4,$str,@eb);

	#
	@eb = LoadList("/home/vai/cgi-bin/cfg/endbar.txt");

	#
	print("
		<table width=$GLOBALW bgcolor=#000000>
		<tr>
		");

	#
	for($i=0; $i<($#eb); $i+=2)
	{
		print("
			<td width=100>
			<a href=\"$eb[1+$i]\" class=bright>
			<div align=center>
				<font size=1>
				<b>
				$eb[0+$i]
				</b>
				</font>
			</div>
			</a>
			</td>
			");
	}
	
	#
	$i = 800-($i/2)*100;
	#
	print("
			<td width=$i>
			</td>
		");

	#
	print("
		</tr>
		</table>
		");
}

#
sub WireLogo
{
		my $LOGO;

		#
		if($_[0] ne "x")
		{
			$LOGO = "$IMGBASE/bg2.gif";
		}
		else
		{
			$LOGO = "$_[0]";
		}

                #
		if($so{'printable'} eq "")
		{
	                # ============ Vaihtoehtouutiset -logo. =============
	                #print("<img src=\"$LOGO\" alt=\"Vaihtoehtouutiset\" border=0>    ");
		}
}

#
sub ULTRAPARSE
{
	my ($str,$i,$i2,$i3,$i4,$str2);

	#
	$str = $_[0];
	$str =~ s/^IMAGE:\"(.*)\"/<img src=\"$1\">/g;
	$str =~ s/^LINK:\"(.*)\"/<a href=\"$1\" class=\"dark\" target=\"_blank\">/g;
	$str =~ s/^CLOSELINK\:/<\/a>/g;
	$str =~ s/^STARTBOX(.*):/<table align=\"right\" cellpadding=4 cellspacing=0 width=$1> <tr><td>/g;
	$str =~ s/^ENDBOX\:/<\/td><\/tr><\/table>/g;

	#
#	if($ENV{'CURSEC'} eq "")
#	{
#		return "";
#	}

	#
	if($str =~ /^{\s/)
	{
		#
		$str2 = $str;
		$str2 =~ s/^{\s(.*)/$1/;
		$str2 =~ s/\s$//g;
		$str2 =~ s/<.*?>//g;
		#
		$str = ("
			<table align=right cellpadding=4 cellspacing=0 
				border=0 $str2>
			<tr>
			<td valing=top>
			<font size=1>
			");
	}

	#
	if($str =~ /^}/)
	{
		#
		$str = ("
			</td>
			</tr>
			</table>
			");
	}
past:

	#
	return $str;
}

################################################################################
#
sub view_gal
{
	my ($str,$str2);

	#
	$GAL =~ s/\<[^\>]+\>//g;
	$GAL =~ s/[^a-z0-9_]//g;

	#
	$IMGBASE = "$IMAGES_BASE/$GAL";

	#
	$str = custom_gallery();

        #
	return $str;
}

################################################################################
#
sub custom_gallery
{
	my ($i,$i2,$i3,$i4,$str,$str2,@lst,$fn,$desc,$name,$desired_txt_fn,
		@tmp,$fn_links,$admin_html);

	#
	@lst = LoadList("find $ENV{'DOCUMENT_ROOT'}/images/$GAL -maxdepth 1 -type f -name '*.jpg'|");
	@lst = sort @lst;

	#
	$str = ("
			<table width=100% cellpadding=2 cellspacing=0>
		");

	#
	$str = ("$str
		<table width=140 cellpadding=0 cellspacing=0>
		<tr>
		<td>
		");

	#
	$str = ("$str
<TABLE width=100% cellpadding=25 cellspacing=0>
<TR valign=top>
		");
	$fn_links = "$ENV{'DOCUMENT_ROOT'}/images/$GAL/links\.txt";
	if( !(-e $fn_links) )
	{
		@links = LoadList($fn_links);
	}
	for($i=0; $i<($#lst+1); $i++)
	{
		#
		$fn = $lst[$i];
		$fn =~ s/^.*\/([a-zA-Z0-9_\-\.\ ]*\.jpg)$/$1/;
		$name = $fn;
		$name =~ s/\.jpg$//;

		#
		if( -e "$ENV{'DOCUMENT_ROOT'}/images/$GAL/thumbs/th_$fn" )
		{
			$desc = "";
			$desc_html = "";

			#
			$desired_txt_fn = "$ENV{'DOCUMENT_ROOT'}/images/$GAL/$name\.txt";
			if(-e $desired_txt_fn)
			{
				@tmp = LoadList("$desired_txt_fn");
				$desc = $tmp[0];
				$desc_html = (" <DIV>$desc</DIV> ");
				if(NoTracking())
				{
					$admin_html = ("<DIV><A HREF=\"/admin/ifeditart.pl?FILE=$desired_txt_fn&PLAIN=TRUE\" target=\"_blank\">> edit description</A></DIV> ");
				}
			}

			#
			if($i>0 && ($i&1)==0)
			{
				$str = ("$str
				</TR>
				</TABLE>
				<TABLE width=100% cellpadding=25 cellspacing=0>
				<TR valign=top>
					");
			}
	
			#
			$str = ("$str
			<TD width=50%>
			<A HREF=\"$IMGBASE/$fn\" class=dark>
			<IMG SRC=\"$IMGBASE/thumbs/th_$fn\" title=\"$desc\" alt=\"$desc\">$desc_html$admin_html
			</A>
			</TD>
				");
		}
		else
		{
			$str = "$str not found $fn<br>\n";
		}
	}

	#
	for($i=0; $i<($#links+1); $i++)
	{
		if($links[$i]=~/^\s*$/) { goto skip; }
		$str = ("$str<A HREF=\"\"><LI><A HREF=\"$links[$i+1]\">$links[$i]</A></LI></A>\n");
		$i++;
skip:
	}

	#
	$str = ("$str
		</TR>
		</TABLE>

		</td>
		</tr>
		</table>
		");

	#
	return $str;
}

#################################################################################
#
sub BuildQuickUrl
{
        my ($v,$i,$i2,$str,$str2,$str3,$str4,@sec,@sp,
		$found,@glose,$org);
        my ($f,@lst);

	#
	if( !($_[0] =~ /x/) )
	{
		return $_[0];
	}

        #
	if($#glose <= 0)
	{
		@glose = LoadList("sections.txt");
	}

        #
	$str = $_[0];
	$org = $_[0];
	$str =~ s/kaikki//g;
	$str =~ s/\/\.\.//g;
	#
	$str =~ s/^\///;
	@sp = split("\/", $str);
	$str2 = $sp[0];
	#
	$str3 =	$str;
	if($str3 =~ /\S*\.\S*\.txt/)
	{
		return "/article/$org";
	}
	else
	{
		$str3 =~ s/^.*pub_artikkeli(.*)\.txt$/$1/;
	}

	#
	loop1: for($i=0,$found=-1; $i<($#glose+1); $i++)
	{
		if($glose[$i] eq $str2)
		{
			$found = $i;
			last loop1;
		}
	}

	#
	if($found==-1)
	{
		return "/404.pl";
	}

	#
	$str = "x";
	return "/?id=$found$str$str3";
}

#################################################################################
#
sub ResolveQuickUrl
{
        my ($v,$i,$i2,$str,$str2,$str3,$str4,@sec);

	#
	if( !($_[0] =~ /x/) )
	{
		return $_[0];
	}

	#
	if($#SECTIONS1<2)
	{
		@SECTIONS1 = LoadList("sections.txt");
	}

        #
	$str2 = "$_[0]";
	$str2 =~ s/^.*=(.*x.*)$/$1/;
        @v = split("x", $str2);
	$i = sprintf "%d", $v[0];

        #
	$str = "$SECTIONS1[$i]/pub_artikkeli$v[1].txt";

	# No spaces allowed.
	$str =~ s/\s//g;

	#
	return $str;
}

################################################################
#
sub OPENTABLE
{
	my ($WIDD);

	#
	$COL = $_[0];

	#
	if($_[1] eq "")
	{
		$WIDD = "100%";
	}
	else
	{
		$WIDD = $_[1];
	}

	#
	print("
		<table cellpadding=2 cellspacing=0
			width=$WIDD>
		<tr>
		<td witdh=100% height=1 bgcolor=\"$COL\">



		<table cellpadding=0 cellspacing=0
			width=100%
			bgcolor=\"#F0FFFF\">
		<tr>

		<td>

		<table cellpadding=4 cellspacing=0 bgcolor=\"#E0FFFF\" width=100%>
		<tr>
		<td>
		");
}

################################################################
#
sub CLOSETABLE
{
	my ($WIDD);

	#
	$COL = $_[0];

	#
	if($_[1] eq "")
	{
		$WIDD = "100%";
	}
	else
	{
		$WIDD = $_[1];
	}

	#
	print("

		</td>
		</tr>
		</table>
		</td>
		</tr>
		</table>


		</td>
		</tr>
		</table>

		");
}

################################################################
#
sub EXPRESS
{
	my ($ALG,$BAK);

	#
	if($_[2] eq "")
	{
		$ALG = "left";
	}
	else
	{
		$ALG = $_[2];
	}

	#
	if($_[3] eq "")
	{
		$BAK = $BGBAK1;
	}
	else
	{
		$BAK = $_[3];
	}

	#
	print("
		<table cellpadding=4 cellspacing=0 width=100% height=28
			bgcolor=\"#2080C0\">
		<tr>
		<td>

		<a href=\"$_[1]\" class=dark>
		<div align=$ALG>
		<font color=\"#0000FF\" size=1>
		$_[0]
		</font>
		</div>
		</a>

		</td>
		</tr>
		</table>
		");
}

################################################################
#
sub EXPRESS2
{
	my ($ALG,$BAK);

	#
	if($_[2] eq "")
	{
		$ALG = "left";
	}
	else
	{
		$ALG = $_[2];
	}

	#
	if($_[3] eq "")
	{
		$BAK = $BGBAK3;
	}
	else
	{
		$BAK = $_[3];
	}

	#
	print("
		<table cellpadding=4 cellspacing=0 width=100% height=26
			bgcolor=\"#2080C0\"
			background=\"$BAK\">
		<tr>
		<td>

		<a href=\"$_[1]\" class=dark>
		<div align=$ALG>
		<font color=\"#80F0C0\" size=2>
		$_[0]
		</font>
		</div>
		</a>

		</td>
		</tr>
		</table>
		");
}

##########################################################
#
sub creation_date
{
        my ($testf,$taika);
        my ($Second, $Minute, $Hour, $Day, $Month, $Year,
                $WeekDay, $DayOfYear, $IsDST);

        #
        if( open($testf,"$_[0]") )
        {
                close($testf);
                ($Second, $Minute, $Hour, $Day, $Month, $Year, $Weekday, $DayOfYear, $IsDST) = localtime(time);
        #       $taika = ctime(creation_date1($_[0]));
                $aika = "$Second";
                ##$aika = "$Day/$Month/$Year  $Hour:$Minute:$Second";

        }
        $aika;
}

##########################################################
#
sub CrDate
{
        my ($testf,$taika,$aika);
        my ($Second, $Minute, $Hour, $Day, $Month, $Year,
                $WeekDay, $DayOfYear, $IsDST);

        #
        if( open($testf, "$_[0]") )
        {
                close($testf);
                $taika = CreationDate1($_[0]);
                $aika = ctime($taika);
        #       ($Second, $Minute, $Hour, $Day, $Month, $Year, $Weekday, $DayOfYear, $IsDST) = localtime($taika);
        #       $aika = "$Day/$Month/$Year  $Hour:$Minute:$Second";
        #       $aika = $taika;

        }
        return $aika;
}

#
sub CreationDate1
{
        my ($aika);

	#
	$aika = 0;

	#
        if( -e $_[0] )
        {
                $aika = (stat($_[0]))[9];
        }
	return $aika;
}

# Tells file age in seconds.
sub FileAge
{
	my ($aik,$taik);

	#
        $taik = time;
        $aik = CreationDate1("$_[0]");
	return $taik-$aik;
}

#
sub Bonus1
{
        my ($aik,$taik,$of,$ero);

        #
        $aik = CreationDate1("$_[0]");
        $taik = time;
	$ero = $taik-$aik;
        if( ($aik+(60*60*12)) > $taik )
        {
                # "NEW ARTICLE" ICON.
                $of = sprintf ("<img src=\"$IMGBASE/uusi.gif\" alt=\"* New! *\" align=\"center\" title=\"$ero\"
				title=\"Artikkeli on uusi!\" border=0>
                ");
                return $of;
        }
        else
        {
                if( ($aik+(60*60*48)) > $taik )
                {
                        # "PRETTY NEW ARTICLE" ICON.
                        $of = sprintf("
                                        <img src=\"$IMGBASE/uusi2.gif\" alt=\"* Pretty new! *\" align=\"center\"
					title=\"$ero\">
                        ");
                        return $of;
                }
        }
        return "";
}

#
sub NumberToDate
{
	my ($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST,$str);

	#
	($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime($_[0]);

	#
	$Year += 1900;
	$Month++;

	#
	$Hour = sprintf "%.2d", $Hour;
	$Minute = sprintf "%.2d", $Minute;
	$Second = sprintf "%.2d", $Second;

	#
	$str = "$Day.$Month.$Year - $Hour:$Minute:$Second";

	#
	return $str;
}

#
sub IsBanned
{
	my ($host);

	#
	$host = $_[0];

	#
	if($host eq "vunet.org"
		|| $host =~ /\.dl\.dl\.cox\.net/i
		|| $host =~ /googlebot/i
		|| $host =~ /msnbot/i
		|| $host =~ /search\.com/i
		|| $host =~ /user-200-47/i
		|| $host =~ /public.alexa.com/i
		|| $host eq "")
	{
		return 1;
	}

	#
	return 0;
}

# so varset
sub VarSet
{
	my ($str,@tmp,$varname,$varvalue);

	#
	$str = $_[0];
	if(!($str=~/\=/)) { return; }
	@tmp = split("\=", $str);
	$varname = $tmp[0];
	$varvalue = $tmp[1];
	if($varname ne "" && $varvalue ne "")
	{
		$so{$varname} = $varvalue;
	}

	#
}

#
sub rot
{
	my $rot13 = new Crypt::Rot13;

	#
	$rot13->charge ($_[0]);
	return $rot13->rot13();
}

##########################################################
#
sub RollThrough
{
        my ($i,$i2,$i3,$i4,$str,$str2,@l);

	#
	@l = LoadList($_[0]);
	for($i=0; $i<($#l+1); $i++)
	{
		VarSet($l[$i]);
	}

	#
}

##########################################################
#
sub PURL
{
        my ($U);

        #
        $U = $_[0];
        if(!($U=~/\=|\&/) && !NoTracking() && !($U=~/\.nsv$/i) )
        {
                $U=sprintf "http://www.vunet.org.cn/?to=%s", $U;
        }
        return $U;
}

#
sub IsImage
{
	#
	if($_[0]=~/\.jpg$/) { return 1; }
	if($_[0]=~/\.gif$/) { return 1; }
	if($_[0]=~/\.png$/) { return 1; }
	if($_[0]=~/\.jpeg$/) { return 1; }

	#
	return 0;
}

#
sub tyhja
{
	#
	if($_[0]=~/^\s$/ || $_[0] eq "") { return 1; }
	return 0;
}

#
sub Special
{
	#
	if($_[0] =~ /http\:\/\//) { return 1; }
	if($_[0] =~ /embed/) { return 1; }
	return 0;
}

#
sub MakeSubject1
{
        my ($ap,@sp);

        #
        $ap = $_[0];
	#
	if($ap=~/<body/i)
	{
	}
        #
        $ap =~ s/\n/ /g;
        $ap =~ s/<script .*>.*<\/script>//gi;
        $ap =~ s/<.*?>//g;
        #
        $ap =~ s/^(.{240}).*$/$1/;
        $ap =~ s/\<//g;
        $ap =~ s/\>//g;
        $ap =~ s/\=//g;
        $ap =~ s/(.{80})/$1\-<br>/g;
        $ap =~ tr/[A-Zï¿½ï¿½ï¿½]/[a-zï¿½ï¿½ï¿½]/;
        $ap =~ s/(^.*)<br>$/$1/i;


        #
        return $ap;
}

#################################################################################33
#
sub pre
{
	my ($ap);

	#
	
}

#################################################################################33
#
sub preview
{
	my ($ap);

        #------------------------------------------------------------
        # GENERATE ARTICLE PREVIEW
        #------------------------------------------------------------
        #
        #
        $ap = $_[0];
        $ap =~ s/\s*$//;
        $ap =~ s/^\s*//;
        $ap =~ s/\t//g;
        $ap =~ s/\_/ /g;
        $ap =~ s/\ \ / /g;
	$ap =~ s/\-\-//g;
	$ap =~ s/\[.*?\]//g;
        #
        $ap =~ s/^(.{240}).*$/$1/;
	$ap = "$ap";

        #
        if($ap =~ /^\s*$/ || $ap eq "")
        {
                $ap = "No preview available";
        }

	#
	return $ap;
}

#
sub IsRunning
{
	my (@lst,$i,$i2,$i3,$i4);

	#
	@lst = LoadList("ps aux|grep $_[0]|grep -v pico|grep -v lynx|grep -v links|");

	#
	return $#lst;
}

#
sub UrlFix
{
	my ($url,$str);

	#
	$url = $_[0];
	$str = "$ENV{'DOCUMENT_ROOT'}/articles/";
	$url =~ s/^$str//;
	if(	$url=~/pub_artikkeli/
		||
		$url=~/[0-9]*\.[0-9]*\.txt$/)
	{
		$url =~ s/\.txt$/\.html/;
		$url =~ s/\/pub_artikkeli([0-9]*\.html)$/\/story-$1/;
	}
	return $url;
}

############################################################################
#
sub ViewCachePage
{
        my ($i,$i2,$str,@lst,$reu);

        #
#        print "Content-type: text/html\n\n";
        # Send error messages to the user, not system log
#        open(STDERR,'<&STDOUT');  $| = 1;

	#
	if($_[0] eq "")
	{
#		$reu = "index-finnish.html";
	}
	else
	{
		$reu = $_[0];
	}

        #
        @lst = LoadList("cache/$reu");

	#
	if($#lst<2)
	{
		print ("Redirection. $reu<BR>");
		print ("Please wait.<BR>
<meta http-equiv=\"refresh\" content=\"0; url=http://www.vunet.org.cn/uutiset/\">
");
	}

        #
        for($i=0; $i<($#lst+1); $i++)
        {
                print "$lst[$i]\n";
        }
}

##########################################################
#
sub ParseOptions
{
        my ($i,$f,@ql,$quoteit,$tep,$author,$cap,@s);

	#
	$so{'imageurl'} = "";
	$so{'imageurl2'} = "";
	$so{'imageurl3'} = "";
	$so{'imageurl4'} = "";

	#
	if( !(-e $_[0]) ) { return; }

        # Got image url!
        @iminfo = LoadList("$_[0]");
        if( !($iminfo[0]=~/\=/) ) { $so{'imageurl'} = $iminfo[0]; }

        #
        for($i=0; $i<($#iminfo+1); $i++)
        {
                if($iminfo[$i]=~/\=/)
                {
                        VarSet($iminfo[$i]);
                }
        }

        #
}

################################################################################################################
#
sub VideoSearchLinks
{
	my ($i,$i2,$i3,$i4,$str,$str2,$str3,$str4,$str5,$str6,$bg,$width,@sp,$pl,$url,$cnt);
	my @items = (
		"All",
		"",
		"find.gif",

		"Saddam",	# TITLE
		"Iraq Saddam",  # KEYWORD 
		"find.gif",

		"Iran",	# TITLE
		"iran",  # KEYWORD 
		"find.gif",

		"Soviet",
		"soviet",
		"find.gif",

		"Russian",
		"russia",
		"find.gif",

		"DPRK",
		"dprk",
		"find.gif",

		"Korean",
		"korea",
		"find.gif",

		"German/DDR",
		"german ddr",
		"find.gif",

		"Funny",
		"funny",
		"find.gif",

		"Sexy",
		"sex girl girls woman boobs tits:dprk north korea china",
		"find.gif",

		"Chinese",
		"china chinese",
		"find.gif",

		"Military",
		"tank mig army",
		"find.gif",

		"Karaoke",
		"karaoke",
		"find.gif",

		"Images",
		"http://www.vunet.org.cn/imgbank.pl?l=en",
		"find.gif",

	#	"Music/MP3",
	#	"http://www.vunet.org.cn/mp3machine.pl?l=en",
	#	"find.gif",

		);

		#
		$pl = 5;

                #
		$str = "";

		#
		$str = ("$str
<!--- START OF VIDEO BAR: --->
		<DIV>
		<TABLE cellpadding=0 cellspacing=0 width=100%
			bgcolor=#C00000
			height=32>
		<TR>
		<TD width=8%>
		<DIV align=center>
		<IMG SRC=\"$IMAGES_BASE/small_red_flag.gif\">
		</DIV>
		</TD>
		<TD width=92% valign=center>
		<DIV align=center>

<TABLE width=100% cellpadding=0 cellspacing=0>
");

	#
	$i2 = sprintf "%d", 800/($#items/3);

	#
	$pro = sprintf "%d", 75/($#items+1);

	#
	$width = 100;

	#
	for($i=0,$cnt=0; $i<$#items; $i+=3)
	{
		#
		if( ($cnt%$pl)==0 )
		{
			$str = ("$str<TR>");
		}

		$str2 = "";
		if($str2 eq "") { $str2 = $ARTCAP; }
		if($str2 eq "") { $str2 = $so{'q'}; }
		if($str2 eq "") { $str2 = $so{'article'}; }

		@splitti1 = split( /\:/, $items[$i+1] );
		@sp  = split(/\ /, $splitti1[0]);
		@sp2 = split(/\ /, $splitti1[1]);
		$bg = "#C00000";
		if($str2 ne "")
		{
			loop: for($i2=0; $i2<($#sp+1); $i2++)
			{
				$str3 = $sp[$i2];
				if($str2 =~ /$str3/i)
				{
					$bg = "#00C0C0";
					$str6 = "keyword_$items[$i+0]";
					$so{$str6}++;
					$hao++;
				}
				$str3 = $sp[$i2];
				if($str2 =~ /$str3/i)
				{
					$bg = "#00C0C0";
					$str6 = "keyword_$items[$i+0]";
					$so{$str6} = 0;
					$buhao++;
					last loop;
				}
			}
		}

		if($items[$i+1]=~/^http:\/\//)
		{
			$url = "$splitti1[0]";
		}
		else
		{
			$url = "/?q=$splitti1[0]&section=videos&FP_SECTION=finnish&maxts=80";
		}

		$str = ("$str
<TD width=$width bgcolor=$bg
	onMouseOver=\"this.className='td_over_black';\"
	onMouseOut=\"this.className='';\"
	onClick=\"window.location='$url';\"
	height=20>
<DIV ALIGN=CENTER>
<IMG src=\"$IMAGES_BASE/$items[$i+2]\" class=bulletin border=0>
<FONT size=2 face=Times Roman color=#F0F000>$items[$i+0]</FONT>
</DIV>
</TD>


			");

		#
		if( ($cnt%$pl)==($cnt-1) )
		{
			$str = ("$str</TR>");
		}

		#
		$cnt++;
	}

	#
	if( ($cnt%$pl)!=0 )
	{
		$str = ("$str</TR>");
	}

	#
	$str = ("$str
</TABLE>

		</DIV>
		</TD>


		</TR>
		</TABLE>
		</DIV>


<!--- END OF VIDEO BAR --->
");

	#
	return $str;
}

#########################################################################################
#
sub Googler
{
	my ($str);

	#
	$str = $_[0];
        $str =~ s/[ï¿½ï¿½]/a/g;
        $str =~ s/[ï¿½ï¿½]/o/g;
        $str =~ s/[^a-zA-Z0-9\ ]//g;
        $str =~ s/\ /\_/g;
        $str =~ s/^(.{20}).*$/$1/;
	return $str;
}

#
sub AddAdmin
{
        my ($rad,$i);

        #
        system "touch $ENV{'DOCUMENT_ROOT'}/articles/cfg/notracking.txt";
        @igl = LoadList("$ENV{'DOCUMENT_ROOT'}/articles/cfg/notracking.txt");

        #
        $rad = $ENV{'REMOTE_ADDR'};
        for($i=0; $i<($#igl+1); $i++)
        {
                if($igl[$i] eq $rad)
                {
                ####    print "IS ON LIST $rad";
                        # His IP is already on the list.
                        return 1;
                }
        }

        # His IP is not yet on the list, let's add it.
        system "echo \"$ENV{'REMOTE_ADDR'}\" >> $ENV{'DOCUMENT_ROOT'}/articles/cfg/notracking.txt";

        #
        return 0;
}

#########################################################################################
#
sub AutoDetermineImage
{
	my ($i,$i2,$i3,$i4,@lst,@lst2,$str,$str2,@sp,@sp2,$cap);
	# Regard following words as important and redirect to correct target.
	my @important=(
		"putin:putin",
		"FUX:busheyes",
		"fox:busheyes",
		"army:usa",
		"bush:busheyes.jpg",
		"medicare:crbaby",
		"pentagon:usoutsm",
		"republican:bushads",
		"republicans:bushads",
		"gop:bushheil",
		"attack:explosion",
		"activist:demonstrators",
		"judge:justice",
		"judges:justice",
		"middle-class:money",
		"dvd:disc",
		"cd:disc",
		"celeron:computer",
		"cuba:cuba.jpg",
		"ecuador:ecuador",
		"ashcroft:ashcroft",
		"chavez:chavez.jpg",
		"oaxaca:oaxaca",
		"greek:greece",
		"denial:denial",
		"white house:white house",
		"elektroniikka:electronics",
		"tietoliikenne:computer",
		"tï¿½itï¿½:proletariat",
		"tï¿½ihin:proletariat",
		"halpa:money2",
		"ammattilaisia:computer",
		"ohjelmistosuunnittelija:computer",
		"ohjelmistosuunnittelijoita:computer",
		"ohjelmoija:computer",
		"ohjelmoijia:computer",
		"myyntiassistentti:business",
		"rightard:bushheil",
		"rightards:bushheil",
		"dictatorship:bushheil",
		"nk:dprk2",
		"jobless:homeless",
		"iraq:iraqi",
		"clinton:billclinton",
		"marines:explosion",
		"911:wtc",
		"class:bushads",
		"Great Leader:bush",
		"McCain:bush",
		"mccain:bush",
		);
	# Ignore following words.
	my @igw=(
		"girl","girls","sex", "Great_wall", "george_michael",
		"jari", "communist", "communism",
		"being", "be", "are",
		"news",
		"himself", "flag",
		"on", "for", "ja", "it", "up",
		"and", "make", "tai",
		"or", "do", "vai", "to",
		"is", "not", "the", "an", "a", "the", "today", "east", "top",
		"with", "in", "again", "of", "kk","uk",
		"0", "00", "12", "td", "david", "pro", "family",
		"photo"
		);

	#
	if( keys %ikes <= 0 )
	{
		# Collect keywords from file names.
		@lst = LoadList("$ENV{'DOCUMENT_ROOT'}/images/cache/IMGcache.txt");

		#
		for($i=0; $i<($#lst+1); $i++)
		{
			$lst[$i] =~ s/^.*\/([^\/]*)$/$1/;
			$str = $lst[$i];
			$str =~ s/\.jpg$//;
			$str =~ tr/[A-Zï¿½ï¿½ï¿½]/[a-zï¿½ï¿½ï¿½]/;
			@sp = split(/[^0-9a-zA-Zï¿½ï¿½ï¿½ï¿½ï¿½ï¿½]/, $str);
			for($i2=0; $i2<($#sp+1); $i2++)
			{
				$str = $sp[$i2];
				# Search for words to ignore.
				loopz: for($i3=0,$i4=0; $i3<($#igw+1); $i3++)
				{
					if($str=~/^$igw[$i3]$/i) { $i4=1; last loopz; }
				}
				if(length($str)>1 && !$i4)
				{
					$ikes{$str} = $lst[$i];
				}
			}
		}
	}

	#
	$cap = $_[0];
	$cap =~ tr/[A-Zï¿½ï¿½ï¿½]/[a-zï¿½ï¿½ï¿½]/;
	@sp2 = split(/[^a-zA-Z0-9ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½]/, $cap);

	#
	for($i=0; $i<($#important+1); $i++)
	{
		@sp = split(/\:/, $important[$i]);
		for($i2=0; $i2<($#sp2+1); $i2++)
		{
			if($sp2[$i2] =~ /^$sp[0]$/i && $sp[1] ne "")
			{
				if($sp[1]=~/\.jp.g$/i) { return "$IMAGES_BASE/$sp[1]"; }
				@sp = ($sp[1]);
				goto past;
			}
		}
	}
 	# Split caption to string array.
	@sp = split(/[^a-z0-9ï¿½ï¿½ï¿½]/, $cap);
past:
	$i=0;

	#
	loop: for($i=0; $i<($#sp+1); $i++)
	{
		$str = $sp[$i];
		if($str ne "" && $sp[$i]=~/^[a-zA-Zï¿½ï¿½ï¿½ï¿½ï¿½ï¿½]+.*$/)
		{
			if( $ikes{$str}=~/\.jpg$/ )
			{
				if($ikes{$str}=~/kim/i)
				{
					if( !($cap=~/dprk/i) ) { goto past2; }
				}
				#print "$ikes{$str}\n";
				return "$IMAGES_BASE/$ikes{$str}";
past2:
			}
			else
			{
			}
		}
	}

	#
	return "";
}

#########################################################################################
#
sub AutoDetermineMapImage
{
	my ($i,$i2,$i3,$i4,@lst,@lst2,$str,$str2,@sp,@sp2,$cap);
	my @important=(
		"kdkt\\S*:locations/LocationNorthKorea.png",
		"valko\\S*:LocationBelarus.jpg",
		"venï¿½jï¿½\\S*:LocationRussia.jpg",
		"venï¿½l\\S*:LocationRussia.jpg",
		"ranska\\S*:LocationFrance.jpg",
		"euro\\S*:LocationEurope.jpg",
		"eu:LocationEurope.jpg",
		"yhdysval\\S*:LocationUSA.jpg",
		"usa:LocationUSA.jpg",
		"suom\\S*:LocationFinland.jpg",
		"saksa\\S*:LocationGermany.jpg",
		"kuuba\\S*:LocationCuba.jpg",
		"iran\\S*:LocationIran.jpg",
		"irak\\S*:LocationIraq.jpg",
		"iraq\\S*:LocationIraq.jpg",
		);
	my @igw=(	# ignore words
		);

	#
	if($so{'geoloc'} ne "")
	{
		$str = $so{'geoloc'};
		$str =~ s/ //g;
		return "$IMAGES_BASE/locations/thumbs/th_Location$str.jpg";
	}

	#
	if( keys %mapkes <= 0 )
	{
		# Collect keywords from file names.
		@lst = LoadList("find $ENV{'DOCUMENT_ROOT'}/images/locations/thumbs -name '*.jpg' -type f -maxdepth 1|");

		#
		for($i=0; $i<($#lst+1); $i++)
		{
			$lst[$i] =~ s/^.*\/([^\/]*)$/$1/;
			$str = $lst[$i];
			$str =~ s/\.jpg$//;
			$str =~ tr/[A-Zï¿½ï¿½ï¿½]/[a-zï¿½ï¿½ï¿½]/;
			@sp = split(/[^0-9a-zï¿½ï¿½ï¿½]/, $str);
			for($i2=0; $i2<($#sp+1); $i2++)
			{
				$str = $sp[$i2];
				# Search for words to ignore.
				loopz: for($i3=0,$i4=0; $i3<($#igw+1); $i3++)
				{
					if($str=~/^$igw[$i3]$/i) { $i4=1; last loopz; }
				}
				if(length($str)>1 && !$i4)
				{
					$mapkes{$str} = $lst[$i];
				}
			}
		}
	}

	#
	$cap = $_[0];
	$cap =~ tr/[A-Zï¿½ï¿½ï¿½]/[a-zï¿½ï¿½ï¿½]/;
	@sp2 = split(/[^a-z0-9ï¿½ï¿½ï¿½]/, $cap);

	#
	for($i=0; $i<($#important+1); $i++)
	{
		@sp = split(/\:/, $important[$i]);
		for($i2=0; $i2<($#sp2+1); $i2++)
		{
			if($sp2[$i2] =~ /^$sp[0]$/i && $sp[1] ne "")
			{
				if($sp[1]=~/\.jpg$/) { return "$IMAGES_BASE/locations/thumbs/th_$sp[1]"; }
				@sp = ($sp[1]);
				goto past;
			}
		}
	}
 	# Split caption to string array.
	@sp = split(/[^a-z0-9ï¿½ï¿½ï¿½]/, $cap);
past:
	$i=0;

	#
	loop: for($i=0; $i<($#sp+1); $i++)
	{
		$str = $sp[$i];
		if($str ne "" && $sp[$i]=~/^[a-zA-Zï¿½ï¿½ï¿½ï¿½ï¿½ï¿½]+.*$/)
		{
			if( $mapkes{$str}=~/\.jpg$/ )
			{
				if($mapkes{$str}=~/kim/i)
				{
					if( !($cap=~/dprk/i) ) { goto past2; }
				}
				return "$IMAGES_BASE/locations/thumbs/$mapkes{$str}";
past2:
			}
		}
	}

	#
	return "";
}

#
# Google (Googlebot.com) / Yahoo (Inktomisearch.com)...
#
sub isRobot
{
	#
	if( $ENV{'HTTP_USER_AGENT'} =~ /centiverse/i )
	{
		return 1;
	}
	#
	#
	if( $ENV{'HTTP_USER_AGENT'} =~ /amppari/i )
	{
		return 1;
	}
	#
	if( $ENV{'HTTP_USER_AGENT'} =~ /robot\.html/i )
	{
		return 1;
	}
	#
	if( $ENV{'HTTP_USER_AGENT'} =~ /Teoma/i )
	{
		return 1;
	}
	#
	if( $ENV{'HTTP_USER_AGENT'} =~ /ZyBorg/i )
	{
		return 1;
	}
	#
	if( $ENV{'HTTP_USER_AGENT'} =~ /HTTrack/i )
	{
		return 1;
	}
	if( $ENV{'HTTP_USER_AGENT'} =~ /crawler/i )
	{
		return 1;
	}
	if( $ENV{'HTTP_USER_AGENT'} =~ /spider/i )
	{
		return 1;
	}
	if( $ENV{'HTTP_USER_AGENT'} =~ /ia_archiver/i )
	{
		return 1;
	}
	if( $ENV{'HTTP_USER_AGENT'} =~ /msnbot\.htm/i )
	{
		return 1;
	}
	if( $ENV{'HTTP_USER_AGENT'} =~ /^msnbot\/[0-9]*\.[0-9]\s\(.*\)$/i )
	{
		return 1;
	}
	if( $ENV{'REMOTE_HOST'}=~/^cache\-out.*\.inet\.fi$/ )	{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/^crawl/ )			{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/vunet\.org$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/vunet\.org.cn$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/coolpages.tk$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/searchme\.com$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/googlebot\.com$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/search\.live\.com$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/inktomisearch\.com$/ )	{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/\.yahoo\.net$/ )		{ return 1; }
	if( $ENV{'REMOTE_HOST'}=~/\.scoutjet\.com$/ )		{ return 1; }

	#
	return 0;
}

#
sub inc_menu
{
	my ($i,$i2,@lst,@lst2,$str,$str2);

	#
	$ENV{'MENUSEC'} = $_[0];
	$ENV{'MENUFPSEC'} = $_[1];
	$str = MainMenu();

	#
	return $str;
}

##################################################################################################
#
sub GetAvlog
{
	my ($TIME_X,$TIME_X2,@lst,$i,$i2,$t,@avlog);

	#
	$t = time - (60*60*24);
	# ip,web_browser,time,referer,url,article_title,local_file_name,cookie_info,host,uid
	my $query = "SELECT time, referer, article_title, ip, host, web_browser, url, local_file_name FROM visitors ORDER BY ID DESC";
	#print "<LI>$query</LI><BR>";
	$sth = $dbh->prepare($query);
	$sth->execute() or print "failed on $i ($DBI::errstr)\n";
	#
	@avlog = ();
	loop: for($i5=time,$i2=0; (@lq = $sth->fetchrow_array); $i2++)
	{
		if($lq[0] < $t) { last loop; }
		push(@avlog, @lq);
		#print @lq; 
	}
	return @avlog;
}

#
@ITEMS_EN = (
"- News",
"Front page:etusivu",
"/news/",
"Discussion:discussion",
"/forum/",
"Editor's choices:picks",
"/picks/",
"Users favorites:favorites",
"/favorites/",
"Progressive:progressive",
"/progressive/",
"Bush:bush",
"/bush/",

"Iraq:iraq",
"/iraq/",
"Iran:iran",
"/iran/",
"Palestine:palestine",
"/palestine/",
"Russia:russia",
"/russia/",
"Belarus:belarus",
"/Belarus/",

#"Music/MPEG:mp3machineen",
#"/mp3machine.pl?l=en",
#"Images:imgbanken",
#"/imgbank.pl?l=en",
"Videos:videos",
"http://www.youtube.com/user/vunet",
#"Internet Radio",
#"http://shoutcast.com/directory/?s=vunet",
"Popular videos:top100",
"/TopTen.pl?q=video&c=100",
"Download software:software",
"/downloads/",
);

#
@ITEMS_FI = (
"- Pääjutut",
"Etusivu:etusivu",
"/finnish/",
"Keskustelu:keskustelu",
"/keskustelu/",
"Äänestys:vote",
"/pollmain.pl",
"Top 100 artikkelit:topafi",
"/cgi/TopTen.pl?n=video&fp=finnish&section=topafi&t=Suosituimmat Artikkelit",
"Lukijat suosittelevat:suositellut",
"/cgi/suositellut.pl",
"Päivän suosituimmat:suosituimmat",
"/top/",
"USA - suuri utopia:yhdysvallat",
"/usa_utopia.pl",
"Poimitut:picks",
"/picks.pl",
#"MP3-kone:mp3machinefi",
#"/cgi/mp3machine.pl?l=fi",
#"Kuvapankki:imgbank",
#"/cgi/imgbank.pl",
#"Videot:videoz2",
#"/cgi/videos.pl",

"- Osastot",
"Kotimaa:kotimaa",
"/finnish/kotimaa",
"Ulkomaat:ulkomaat",
"/finnish/ulkomaat",
"Politiikka:politiikka",
"/finnish/politiikka",
"Konfliktit:konfliktit",
"/finnish/konfliktit",
"Talous:talous",
"/talous/",
"Tiede/Terveys/IT:tiede",
"/tiede/",
"Videot\/Kulttuuri:kulttuuri",
"/finnish/kulttuuri",

"- Viihde",
"Kummalliset:kummalliset",
"/kummalliset/",
"Videot:videoz",
"/videos/",

"- Yhteiskunta",
"Jutut:yhteiskunta",
"/cgi/nw.pl?FP_SECTION=finnish&section=yhteiskunta",
"Tiedotteet:tiedotteet",
"/cgi/nw.pl?FP_SECTION=finnish&section=tiedotteet",
"Luonto:luonto",
"/cgi/nw.pl?FP_SECTION=finnish&section=luonto",
"Kolumnit:kolumnit",
"/cgi/nw.pl?FP_SECTION=finnish&section=kolumnit",
"Ajatelmat:ajatelmat",
"/cgi/nw.pl?FP_SECTION=finnish&section=ajatelmat",

"- Hallinto",
"Hallintokeskus:hallintokeskus",
"/cgi/admin/center.pl",
"Julkaise artikkeli:julkaise",
"/cgi/publisharticle.pl",
);

#
sub MajorItemBox
{
	my ($str,$str2,$str3,$str4,@lst,@lst2,$con,@items);

	#
	$str = ("
<TABLE width=140 class=MajorItemBox>
<TR>
<TD>
	<A HREF=\"$_[1]\" class=bright>
	<H2 CLASS=head2_selected>
	$_[0]
	</H2>
	</A>
</TD>
</TR>
</TABLE>

		");

	#
	return $str;
}

#
sub SelectedItemBox
{
	my ($str,$str2,$str3,$str4,@lst,@lst2,$con,@items);

	#
	$str = ("
<TABLE width=140 class=SelectedItemBox>
<TR>
<TD>
	<A HREF=\"$_[1]\" class=dark>
	<H2 CLASS=head2_selected>
	$_[0]
	</H2>
	</A>
</TD>
</TR>
</TABLE>

		");

	#
	return $str;
}

#
sub ItemBox
{
	my ($str,$str2,$str3,$str4,@lst,@lst2,$con,@items);

	#
	$str = ("
<TABLE width=140 class=ItemBox>
<TR>
<TD>
	<A HREF=\"$_[1]\" class=bright>
	<H2 CLASS=head2>
	$_[0]
	</H2>
	</A>
</TD>
</TR>
</TABLE>

		");

	#
	return $str;
}

#
sub MainMenu
{
	my ($str,$str2,$str3,$str4,@lst,@lst2,$con,@items,
		$i,$i2,$i3,$i4);
	my @secs = (
	"kuuba", "venezuela", "vietnam", "kiina", "dprk", "laos",
	"libya", "iran", "seksi", "keskustelu", "discussion", "psykologia"
		);

	#
	vis_log();

	#
	for($i=0; $i<($#secs+1); $i++)
	{
		if($secs[$i] eq $so{'rs'})
		{
			$ENV{'MENUSEC'} = $secs[$i];
		}
	}

	#
	if($ENV{'MENUSEC'} eq "")
	{
		$ENV{'MENUSEC'} = "etusivu";
	}
	
	#$str = $ENV{'REQUEST_URI'};
	#$ENV{'MENUSEC'} = $_[0];
	#$ENV{'MENUFPSEC'} = $_[1];
	if($ENV{'MENUFPSEC'} eq "")
	{
		$ENV{'MENUFPSEC'} = "finnish";
	}

	#
	if(1==1 || ($ENV{'MENUFPSEC'} eq "finnish" && $so{'section'} ne "progressive" && $so{'section'} ne "bush"))
	{
		@items = @ITEMS_FI;
	}
	else
	{
		@items = @ITEMS_EN;
	}

	#
	$con = "";

	#
	loop2: for($i=0,$tested=0,$t=time(); $i<($#items+1); $i++)
	{
		#
		$str = $items[$i];

		if( (time()-$t)>=4 ) { $con = $con . "time-out<BR>"; last loop2; }
		#
		if($str=~/^-/)
		{
			$con = $con . MajorItemBox("$str", $items[$i+2]);
			$tested = 0;
			$i3 = $i;
		}
		else
		{
			if($tested==0)
			{
				loop: for($i2=$i; $i2<($#items+1); $i2++)
				{
					if($items[$i2]=~/^-/ && $i3!=$i2)
					{
						$i = $i2-1;
						goto past;
					}
					@sp = split(/\:/, $items[$i2+0]);
					if($sp[1] eq $ENV{'MENUSEC'} && $sp[1] ne "") { last loop; }
					$i2++;
				}
			}
			$tested = 1;

			@sp = split(/\:/, $items[$i+0]);
			if($sp[1] eq $ENV{'MENUSEC'} && $sp[1] ne "")
			{
				$con = $con . SelectedItemBox($sp[0], $items[$i+1]);
			}
			else
			{
				$con = $con . ItemBox($sp[0], $items[$i+1]);
			}
			$i++;
		}
past:
	}

	#
	$con = ($con . "
<TABLE width=100% height=8 bgcolor=#800000>
<TR>
<TD>
</TD>
</TR>
</TABLE>
");

	#
	return $con;
}

#
sub Rot13URL
{
	my ($code1,$code2,$URL);

	#
	$URL = $_[0];
	$URL =~ s/pub_artikkeli/story-/;
	$URL =~ s/\.txt$/.html/;
	$URL =~ s/^[a-z]\///;
	$OURL  = $URL;
	$code1 = $URL;
	$code1 =~ s/^([a-z]*).*$/$1/;
	$code2 = $URL;
	$code2 =~ s/^.*\-([0-9\.]*)\..*$/$1/;

	#
	$code1 =~ y/A-J/0-9/;
	$code1 =~ y/a-z/n-za-m/;
	$code1 =~ y/0-45-9/5-90-4/;
	$code2 =~ y/0-45-9/5-90-4/;
	$code2 =~ y/0-9/A-J/;

	#
	$s = $_[0];
	$s =~ s/^([a-z]*)\/.*$/$1/;
	if($URL=~/[0-9]*\.[0-9]*\.html$/ || isAutoSection($s)==1)
	{
		$URL = "http://www.vunet.org.cn/$URL";
	}
	else
	{
		$URL = "http://www.vunet.org.cn/x$code1$code2";
	}

	#
	return $URL;
}

#
TRUE;

