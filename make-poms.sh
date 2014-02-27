for x in `ls -d JCu* | grep -v "JNI"`; do
  lowerName=$(echo $x | tr [A-Z] [a-z] | sed -e 's/java$//')
  echo $x, $lowerName
  sed -e "s/lpsolve/$lowerName/" pom-template.xml > $x/pom.xml
done
