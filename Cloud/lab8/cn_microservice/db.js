const mysql = require('mariadb')
//const endpoint  = require('./endpoint')

const pool = mysql.createPool({
//		host: endpoint,
		host: 'node-cc-app-id.cnjl6jnnj86z.ap-south-1.rds.amazonaws.com',
		user: 'user',
		password: 'user1234',
		waitForConnection:true,
		connectionLimit:10,
		queueLimit:0
});

module.exports = pool
