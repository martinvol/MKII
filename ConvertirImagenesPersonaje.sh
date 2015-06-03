PATHJUGADOR="resources/jugador"
LISTADEIMAGENES=(`find $PATHJUGADOR -name "*.png"`)

for IMAGEN in ${LISTADEIMAGENES[*]}
do
    convert $IMAGEN -colors 256 PNG8:$IMAGEN
done


