all: __make-lib __make-test __make-app

debug: __make-lib __make-test __make-app

clean: __make-lib __make-test __make-app

re: __make-lib __make-test __make-app

__make-lib:
	@echo "Building library..."
	@$(MAKE) -C ./lib $(MAKECMDGOALS)
	@echo "Completed"

__make-test:
	@echo "Building tests..."
	@$(MAKE) -C ./unit_tests $(MAKECMDGOALS)
	@echo "Completed"

__make-app:
	@echo "Building app..."
	@$(MAKE) -C ./pos $(MAKECMDGOALS)
	@echo "Completed"

.PHONY: all debug clean re
