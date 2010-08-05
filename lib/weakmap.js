var EphemeronTable = require("./overload").EphemeronTable

module.exports = function WeakMap() {
	var backer = new EphemeronTable
	this.set = backer.set
	this.get = backer.get
	return this
}