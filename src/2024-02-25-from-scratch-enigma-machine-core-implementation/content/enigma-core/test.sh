#!/usr/bin/env sh

CMD="set reflector M3-B\n"
CMD+="set rotor left M3-I 0 0\n"
CMD+="set rotor middle M3-II 0 0\n"
CMD+="set rotor right M3-III 0 0\n"
CMD+="set plugboard B-Q C-R\n"
CMD+="info\n"
CMD+="encrypt DSFSDFSDF\n"
CMD+="quit\n"

echo -e "${CMD}" | ./examples/cli | grep -q MXUNIBVUQ
[ $? != 0 ] && echo "ERROR: invalid!" && exit;

echo "All good!"
