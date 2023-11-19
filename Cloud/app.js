const express = require('express');
const session = require("express-session");
const passport = require("passport");
const auth = require("./auth");
const app = express();
const cors = require('cors');
const proxy = require('express-http-proxy');
var bodyParser = require('body-parser');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use(cors());
app.use(express.json());
app.use('/1/', proxy('http://localhost:8083'));
app.use('/0/', proxy('http://localhost:8082'));
app.use('/', proxy('http://localhost:8081'));
app.use(
  session({ name: "x", secret: "cats", resave: false, saveUninitialized: true })
);
app.use(passport.initialize());
app.use(passport.session());
const port  = process.env.PORT || 80;
console.log(port)

function isLoggedIn(req, res, next) {
  req.user ? next() : res.sendStatus(401);
}

app.get(
  "/auth/google",
  passport.authenticate("google", {
    session: false,
    scope: ["profile", "email"],
    prompt: "select_account",
  })
);

app.get(
  "/auth/google/callback",
  passport.authenticate("google", { failureRedirect: "/auth/google/callback" }),
  (req, res) => {
    res.redirect("/");
  }
);

app.get("/auth/failure", (req, res) => {
  res.send("Authentication failed. Only @iiitg.ac.in domain is allowed.");
});

app.get("/logout", (req, res) => {
  req.logOut(function (err) {
    if (err) {
      return next(err);
    }
    res.redirect("/");
  });

});

function ensureAuthenticated(req, res, next) {
  if (req.isAuthenticated()) {
    return next();
  } else {
    res.redirect("/auth/google");
    // res.redirect("/");
  }
}

app.listen(port, ()=>{
		console.log("started");
});
