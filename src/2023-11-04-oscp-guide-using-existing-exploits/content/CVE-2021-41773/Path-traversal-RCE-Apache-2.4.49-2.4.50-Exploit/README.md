### Exploit for CVE-2021-41773 and CVE-2021-42013
**Path traversal attack and RCE in Apache/2.4.49-2.4.50**

### Features

-> Take a list of urls

-> Works for both CGI and non-CGI

-> Works for Apache/2.4.49 - 2.4.50


### Usage

`$ git clone https://github.com/CalfCrusher/Path-traversal-RCE-Apache-2.4.49-2.4.50-Exploit`

`$ cd Path-traversal-RCE-Apache-2.4.49-2.4.50-Exploit && pip3 install -r requirements.txt`

`$ python3 main.py urls.txt`

### A Brief History 

On the 5th of October 2021, a CVE detailing a path traversal attack on Apache HTTP Server v2.4.49 was released. Assigned the number CVE-2021-41773, it was released with the following description:

    A flaw was found in a change made to path normalization in Apache HTTP Server 2.4.49.
    An attacker could use a path traversal attack to map URLs to files outside the expected document root. If files outside of the document root are not protected by "require all denied" these requests can succeed.
    Additionally (sic) this flaw could leak the source of interpreted files like CGI scripts.
    This issue is known to be exploited in the wild. This issue only affects Apache 2.4.49 and not earlier versions. 

Let's break this down and see what this actually means for us:

    From the first bit, we see that a recent change that exposed the flaw. Path normalization means that we transform a given path to some canonical form that the software can understand, and thus map to the actual filesystem. This already leads us to suspect a path traversal attack that can potentially read unintended files.
    The next part confirms our suspicions, and we are able to use a path traversal attack to read resources outside the intended scope.
    We see that we require a very particular configuration to be set. Files outside the document root must explicitly be granted permissions. This is not the default configuration and should thus render this exploit useless against a large percentage of the Apache hosts (thankfully).
    The next part talks about CGI scripts, which erroneously leads us to believe that CGI may need to be enabled for this attack to work or that the path involves CGI in some way.
    Even if our configuration is not directly affected by this bug, we'll still want to update the vulnerable versions ASAP.

To summarize, in order to exploit this vulnerability, we'll need to have a very unusual configuration on our target server, and to attack via a specific path.
Much Fixing Later...

So Apache fixed this bug and released v2.4.50. End of story, right? Well, not quite. Only 2 days later, on the 7th of October, a new CVE was released citing the prior. This one mentions that the fix for the earlier path traversal attack was incomplete, and we could still traverse if the path in question used an alias directive to map its URLs to the filesystem. The CVE was assigned number CVE-2021-42013, with the description as follows:

    It was found that the fix for CVE-2021-41773 in Apache HTTP Server 2.4.50 was insufficient.
    An attacker could use a path traversal attack to map URLs to files outside the directories configured by Alias-like directives.
    If files outside of these directories are not protected by the usual default configuration "require all denied", these requests can succeed.
    If CGI scripts are also enabled for these aliased pathes (sic), this could allow for remote code execution. This issue only affects Apache 2.4.49 and Apache 2.4.50 and not earlier versions. 

Much as before, we can learn a few things here:

    While the first exploit was supposedly fixed, there is another input for allowing the traversal to work (remember this for later).
    Now we're being limited to aliased path directives.
    Directories outside the usual paths still require explicit permissions to be granted.
    If CGI is enabled, then we can get RCE in addition to simple disclosure

##

*Please note that i'm not responsible for any damages and illegal use. Don't be a twat!*
