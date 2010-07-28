//sharing closures can allow prototypal construction speed times while gaining access to a closure
//and not just any closure, all closures to objects of a given type!
var EphemeronTable = require("../lib/overload").EphemeronTable

var MyType
;(function(){
	var closures = new EphemeronTable;
	MyType = function() {
		var myClosures = {views:0}
		closures.set(this,myClosures)
	}
	var communicate = MyType.prototype.communicate = function(object) {
		return closures.get(object)
	}
	MyType.prototype.view = function() {
		return communicate(this).views++
	}
	MyType.prototype.views = function() {
			return communicate(this).views
	}
	MyType.prototype.syncViewsWith = function(obj) {
		communicate(this).views = communicate(obj).views
	}
})()
var instance_a = new MyType
  , instance_b = new MyType

instance_a.view()
instance_b.syncViewsWith(instance_a)
console.log(instance_b.views())//1

//making new prototypes is still possible!
MyType.prototype.doubleView = (function(){
	var communicate = MyType.prototype.communicate
	return function() {
		communicate(this).views += 2
	}
})()
instance_a.doubleView()

//since communicate is available you can still access closures until it isnt
console.log(MyType.prototype.communicate(instance_a).views)//3
delete MyType.prototype.communicate

//Communicate is no longer available (kinda like sealing)
try{
	console.log(MyType.prototype.communicate(instance_a))//fails
}
catch(e) {
	console.log(e.stack)//error!
}

//Compiled functions keep the key!
var instance_c = new MyType
instance_c.syncViewsWith(instance_a)
instance_c.doubleView()
console.log(instance_c.views())//5