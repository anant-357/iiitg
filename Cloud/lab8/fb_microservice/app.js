const express = require('express');
const app = express();
const pool = require('./db');
var bodyParser = require('body-parser');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use('/', express.static('./'));

const create_database = `CREATE DATABASE IF NOT EXISTS feedback`

const use_database = `USE feedback`

const create_table = `CREATE TABLE IF NOT EXISTS feedback (
		id INT NOT NULL AUTO_INCREMENT,
		name VARCHAR(255) NOT NULL,
		message VARCHAR(255) NOT NULL,
		PRIMARY KEY (id)
)`;


pool.getConnection().then(async (connection) => {
		try {
			await connection.query(create_database);
			await connection.query(use_database);
			await connection.query(create_table);
			connection.commit();
		} catch (err) {
			console.error('Error Creating Table:', err);
		} finally {
			connection.close()
		} 
});

app.get('/feedback', (req, res) => {
		res.sendFile(__dirname + '/feedback.html');
});

app.get('/comments', (req, res) => {
		pool.getConnection().then( async (connection) => {
				try {
						await connection.query(use_database);
						const rows = await connection.query("SELECT name,message from feedback");
						let out = `<html>
								<head><title>Comments</title></head><body><br><br><center>` ;
						for(let i = 0; i < rows.length; i++){
								out = out + rows[i].name + ":" + rows[i].message + "<br><br>";
						}
						out = out + "<a href='/'>Go Back</a></center></body></html>"
						res.send(out);
				} catch (err) {
						console.error("Error retreiving data:",err);
				} finally {
						connection.close();
				} 
		});
});

app.post('/feedback', (req, res) => {
		const name = req.body.name;
		const message = req.body.message;
		pool.getConnection().then( async (connection) => {
				try {
						await connection.query(use_database);
						await connection.query("INSERT INTO feedback (name, message) VALUES (?,?)", [name, message]);
						connection.commit();
						res.redirect("/0/comments");
				} catch (err) {
						console.error("Error inserting data:", err);
						res.status(500).send("Error inserting data.")
				} finally {
						connection.close()
				}
		});
});

const port  = process.env.PORT || 8082;
console.log(port)
app.listen(port, ()=>{
		console.log("started");
});
