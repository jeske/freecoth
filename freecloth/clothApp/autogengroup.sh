#!/bin/sh

# Automatically generate screenshots by varying one parameter and recording.
# Software required:
#   imagemagick - image conversion (convert, montage)
#   ghostscript - graph conversion
#   matlab - graph creation
#   recmpeg & libfame - MPEG recording
#   /usr/bin/time - timing execution

DO_RUN=1
DO_MPEG=1
DO_GRAPH=1

PARAM=$1
VALUES=$2
# Shift those out, leaving extra args in $*
shift; shift;
REALDISPLAY=$DISPLAY

for VALUE in $VALUES; do
    SUBDIR=screenshots/$PARAM/$VALUE
    if [ $DO_RUN -ne 0 ]; then
        rm -rf $SUBDIR
        mkdir -p $SUBDIR
        if [ $PARAM == bend ]; then
            VALUE="$VALUE $VALUE"
        fi
        /usr/bin/time --format %E \
            nice -20 \
            ./clothApp \
                -settings autogenSettings.txt \
                -constraint table_circle \
                -snapPrefix $SUBDIR/ \
                -statsPrefix $SUBDIR/ \
                -nbPatches 66 \
                -timestep 0.01 \
                -maxTimestep 0.005 \
                -crop 4 110 700 450 \
                -stretchLimit 5e-5 \
                -tolerance 0.01 \
                -$PARAM $VALUE \
                -batch 2.5 \
                $* \
            >& $SUBDIR/time.txt
        find $SUBDIR -name "*ppm" | sed 's/\(.*\).ppm/convert \1.ppm \1.png/' | sh
        find $SUBDIR -name "*ppm" | xargs rm
    fi

    if [ $DO_MPEG -ne 0 ]; then
        # Make mpegs
        TYPES="motion energy"
        for TYPE in $TYPES; do
            # Four stages, per frame:
            #   convert - resize to 352x288. Due to aspect ratio of cropped image,
            #       352x226 is what we actually get.
            #   montage - pad to 352x288, unfortunately converting to 16-bits per
            #       channel in the process for some reason.
            #   pnmdepth - convert back to 8-bits per channel
            #   convert - convert to YUV format
            ls -1 $SUBDIR/$TYPE*.png \
                | xargs -i{} \
                    echo 'convert \
                        -filter cubic \
                        -blur 3x1 \
                        -resize 352x226 \
                        -bordercolor black \
                        {} /tmp/mpeg0.png \
                    && montage \
                        -background black \
                        -geometry +0+32 \
                        /tmp/mpeg0.png /tmp/mpeg1.ppm \
                    && pnmdepth \
                        255 /tmp/mpeg1.ppm > /tmp/mpeg2.ppm; \
                    convert \
                        -crop 352x288 \
                        -depth 8 \
                        /tmp/mpeg2.ppm {}.yuv' \
                | sed 's/\.png\.yuv$/.yuv/' \
                | sh \
            && cat $SUBDIR/$TYPE*.yuv \
                | recmpeg --fps 25 --quality 100 \
                > $SUBDIR/${PARAM}-${VALUE}-${TYPE}.mpg \
            && rm -f $SUBDIR/$TYPE*.yuv \
            && rm -f /tmp/mpeg{0,1,2}.ppm
            # Last stages above Merge all the YUV files and make an MPEG
        done
    fi

    if [ $DO_RUN -ne 0 ]; then
        # Make thumbnails
        mkdir -p $SUBDIR/thumb
        find $SUBDIR | grep png \
            | sed 's/\(.*\)\/\([^\/]*\)/convert -resize 150x150 \1\/\2 \1\/thumb\/\2/' \
            | sh
    fi
done

if [ $DO_GRAPH -ne 0 ]; then
    # Clear temporary files
    echo > /tmp/stretch.txt
    echo > /tmp/shear.txt
    echo > /tmp/bend.txt
    echo > /tmp/step.txt

    # Accumulate all stretch/shear/bend values into temp files.
    for VALUE in $VALUES; do
        SUBDIR=screenshots/$PARAM/$VALUE
        if [ -f $SUBDIR/stretch.txt.gz ]; then
            gunzip $SUBDIR/*.txt.gz
        fi
        cat $SUBDIR/stretch.txt >> /tmp/stretch.txt
        cat $SUBDIR/shear.txt >> /tmp/shear.txt
        cat $SUBDIR/bend.txt >> /tmp/bend.txt
        cat $SUBDIR/step.txt >> /tmp/step.txt
    done

    rm -f /tmp/file
    for VALUE in $VALUES; do
        SUBDIR=screenshots/$PARAM/$VALUE

        # Make graphs
        echo "makegraph('$SUBDIR/');" >> /tmp/file
    done
    cat /tmp/file | matlab -nojvm  > /dev/null
    rm -f /tmp/file
    for VALUE in $VALUES; do
        SUBDIR=screenshots/$PARAM/$VALUE

        convert $SUBDIR/graph.eps $SUBDIR/graph.png
        convert $SUBDIR/stepgraph.eps $SUBDIR/stepgraph.png
        rm $SUBDIR/graph.eps $SUBDIR/stepgraph.eps
        # We'll just leave *_big as eps, since they're mostly just used for
        # printing
        gzip $SUBDIR/*.txt
        convert -resize 150x150 $SUBDIR/graph_big.eps $SUBDIR/thumb/graph.png
        convert -resize 150x150 $SUBDIR/stepgraph_big.eps $SUBDIR/thumb/stepgraph.png
    done
fi
