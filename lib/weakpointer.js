var EphemeronTable = require("./overload")
var OnCollect = EphemeronTable.OnCollect
EphemeronTable = EphemeronTable.EphemeronTable

module.exports = WeakPointer(target) {
	var eph_table = new EphemeronTable()
	eph_table.set(target,null)
	var executors = []
	var alive = true
	this.get = function() {
		return alive?eph_table.keys()[0]:undefined
	}
	this.register = function(executor) {
		if(alive) executors.push(executor)
		return executor
	}
	OnCollect(target,function collector() {
		alive=false
		executors.forEach(function(executor){
			setTimeout(executor,0)
		} )
	} )
}