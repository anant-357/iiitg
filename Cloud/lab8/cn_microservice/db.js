const mysql = require('mariadb')
const endpoint  = require('./endpoint')

const pool = mysql.createPool({
		host: endpoint,
		user: 'user',
		password: 'user1234',
		waitForConnection:true,
		connectionLimit:10,
		queueLimit:0
});

module.exports = pool
