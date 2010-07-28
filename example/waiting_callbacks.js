var EphemeronTable = require("../lib/overload").EphemeronTable

var callbacks = new EphemeronTable;

setTimeout(
	callbacks.set(function myCallback() {
		console.log("callback invoked")
	},"simple timeout")
	,5000
)

//get the value of the first callback from the table
console.log(callbacks.get(callbacks.keys()[0]))