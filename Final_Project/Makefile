JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	S1/s1.java \
	C1/c1.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
