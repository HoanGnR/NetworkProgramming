JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Client/client.java \
	Server/server.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
