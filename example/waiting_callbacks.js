var EphemeronTable = require("../lib/overload").EphemeronTable

var callbacks = new EphemeronTable;

setTimeout(
	callbacks.set(function myCallback() {
		console.log("callback invoked")
	},"simple timeout")
	,5000
)

console.log(callbacks.get(callbacks.keys()[0]))