const express = require('express');
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

const port  = process.env.PORT || 80;
console.log(port)
app.listen(port, ()=>{
		console.log("started");
});
