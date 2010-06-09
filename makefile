all:
	@echo "BUILDING: C++ Component"
	@cd src;node-waf configure build;cd ..
	@cp ./src/build/default/overload.node ./lib

clean:
	rm -rf src/build/
	rm -rf lib/overload.node
	rm -rf src/.lock-wscript