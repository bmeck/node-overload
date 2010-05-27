#node-watchable

##exports
* Watchable(onGet,onSet)

		Supplies getter and setter callbacks across all the properties
		Currently both callbacks are required

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