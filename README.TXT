#Node
##exports
* Watchable(onGet,onSet,onForeach)

		Supplies getter and setter callbacks across all the properties along with a for(x in y) callback that returns the list
		Callback's can use the _this_ object in order to act normally without reinvoking themselves.

**	Value onGet(String propertyName, Value value, hadAlready)

		Returns the value at a specific index
**	Value onSet(String propertyName, Value oldValue, Value value, hadAlready)

		Returns the value to save at a specific index
**	Array onForeach()

		Returns an array containing all the index keys for this object



##example
Code

	//print out any property gets that use the . operator
	var debug = Watchable(
		function(propertyName,value,hadAlready){
			//check the type of the object contained by value
			//undefined if no value, or number if using []'s with a number
			if(typeof value === "string") {
				sys.puts(propertyName);
			}
			return value;
		}
		function(propertyName,oldValue,value,hadAlready) {
			return value;
		}
	)
	debug.hello
	debug[" world!"]
Output

	hello
	 world!

##uses
1. Debugging - Show what is being accessed / set
2. False natives - refuse to allow properties to be set / got beyond a specified few
3. Dynamic programming - Fib[3] could compute Fib[1] and Fib[2]
4. Index based getters / setters - Fib[0] cannot have a getter set normally because it is a number not string based index (all non-number non-undefined values become string based).