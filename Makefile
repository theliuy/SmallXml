SmallXml: SmallXml.cpp SmallXml.h
	g++ -c SmallXml.cpp

demo_all: SmallXml.cpp SmallXml.h
	g++ -o Demo_All -DDEMO_SMALLXML -DDEMO_TOSTRING -DDEMO_INSERTS -DDEMO_PARSER -DDEMO_FIND -DDEMO_XPATH SmallXml.cpp

demo_tostring: SmallXml.cpp SmallXml.h
	g++ -o Demo_ToString -DDEMO_SMALLXML -DDEMO_TOSTRING SmallXml.cpp

demo_inserts: SmallXml.cpp SmallXml.h
	g++ -o Demo_Inserts -DDEMO_SMALLXML -DDEMO_INSERTS SmallXml.cpp

demo_parser: SmallXml.cpp SmallXml.h
	g++ -o Demo_Parser -DDEMO_SMALLXML -DDEMO_PARSER SmallXml.cpp

demo_find: SmallXml.cpp SmallXml.h
	g++ -o Demo_Find -DDEMO_SMALLXML -DDEMO_FIND SmallXml.cpp

demo_xpath: SmallXml.cpp SmallXml.h
	g++ -o Demo_XPath -DDEMO_SMALLXML -DDEMO_XPATH SmallXml.cpp

clean_demos: SmallXml.cpp SmallXml.h
	rm Demo_*
  
clean_all: SmallXml.cpp SmallXml.h
	rm -f Demo_*
	rm -f *.o 
