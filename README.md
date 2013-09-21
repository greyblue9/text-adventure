Text Adventure
==============

A parser-based game engine for interactive fiction (Zork-style text-based 
adventure) written in PHP. Partially motivated by a desire to learn how to write
a classic syntax parser (for the game description file), and also a natural
language parser (for user input).  

Example of game syntax it can parse:

	@object
	    @name           computer
	    @title          @a computer
	    @tagline        On top of the desk sits a computer.
	    @description    The computer looks pretty modern--there are two large 
						LCD monitors connected to it, side-by-side. It's turned
						on and there is a sticky note on the desktop: "finish
						game." You are a little puzzled.
	    @nouns			computer
	    @carry?         @no It's called a desktop for a reason.
	    
	@object
	    @name           bed
	    @title          @a bed
	    @description    You're surprised to find such a large bed in a room 
						belonging to a college student.
	    @nouns			bed
	    @carry?         @no
	    
	@object
	    @name           periodic_table
	    @title          @a periodic table
	    @tagline        There is a periodic table plastered on the wall.
	    @description    It's a periodic table, alright. It's got everything from 
						hydrogen to radon, including all the made-up elements 
						such as lawrencium. This one is super shiny!
	    @nouns			table
	    @adjs			periodic
	    @carry?         @yes You roll up the periodic table and stow it away in 
						your jeans.
	
	@object
	    @name           paper_shade
	    @title          @a paper shade
	    @description    It's a temporary shade. Nothing special about it, 
						really.
	    @nouns			shade, blinds, blind
	    @adjs			paper, temporary, window
	    @carry?         @yes You shove the paper shade into your trousers.
