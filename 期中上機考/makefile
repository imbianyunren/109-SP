all:lazybox.c
	gcc lazybox.c --static
	ln -s ./a.out ./ln
	ln -s ./a.out ./chown
	ln -s ./a.out ./stat
	ln -s ./a.out ./lazyshell
	sudo chown root ./a.out
	sudo chmod +s ./a.out
clean:
	rm a.out
	rm ln
	rm chown
	rm stat
	rm lazyshell

