var Overload=require("overload");
var Watchable=Overload.Watchable;
var RePrototype=Overload.RePrototype;
module.exports = {
	create: function(handler,prototype) {
		var proxy=Watchable(
			//get
			function(ArgInfo) {
				return handler["get"].call(ArgInfo.thisObject,ArgInfo.holder,ArgInfo.property)
			}
			//set
			,function(context) {
				return handler["set"].call(context.thisObject,context.holder,context.property,context.value)
			}
			//enum
			,function(context) {
				return handler["enumerate"].call(context.thisObject);
			}
			//query
			,function(context) {
				return !!handler["has"].call(context.thisObject,context.property);
			}
			//delete
			,function(context) {
				return !!handler["delete"].call(context.thisObject,context.property);
			}
			//call
			,null
			//construct
			,null
		);
		RePrototype(proxy,prototype||Object.getPrototypeOf(new Object()));
		return proxy;
	}
}
