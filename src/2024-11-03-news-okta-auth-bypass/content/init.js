const bcrypt = require('bcrypt');

const saltRounds = 10;

bcrypt.genSalt(saltRounds, function(err, salt) {
  
  bcrypt.hash("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABPASSWORD1", salt, function(err, hash) {
    console.log(hash);
  });

  bcrypt.hash("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACPASSWORD2", salt, function(err, hash) {
    console.log(hash);
  });
  
});
