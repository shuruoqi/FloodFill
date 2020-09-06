/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace.
 *
 */

/**
 * Performs a multi-point flood fill using breadth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillBFS(FillerConfig &config) {
    /**
     * @todo Your code here! You should replace the following line with a
     * correct call to fill.
     */
    return fill<Queue>(config);
}

/**
 * Performs a multi-point flood fill using depth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillDFS(FillerConfig &config) {
    /**
     * @todo Your code here! You should replace the following line with a
     * correct call to fill.
     */
    return fill<Stack>(config);
}

/**
 * Run a multi-point flood fill on an image
 *
 * @param  config     FillerConfig struct with data for flood fill of image
 * @return animation  object illustrating progression of flood fill algorithm
 */
template<template<class T> class OrderingStructure>
animation filler::fill(FillerConfig &config) {
    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. To begin the algorithm, you simply place the
     * given point in the ordering structure, marking it processed
     * (the way you mark it is a design decision you'll make yourself).
     * We have a choice to either change the color, if appropriate, when we
     * add the point to the osPoint, or when we take it off. In our test cases,
     * we have assumed that you will change the color when a point is added
     * to the structure.
     * Until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        1.    add its unprocessed neighbors whosPointe color values are
     *              within (or equal to) tolerance distance from the center,
     *              to the ordering structure, and
     *              mark them as processed.
     *        2.    if it is an appropriate frame, send the current PNG to the
     *              animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore neighboring
     *        pixels in some order.
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! The order you should put
     *        neighboring pixels **ONTO** the queue or stack is as follows:
     *        ** LEFT(-x), DOWN(+y), RIGHT(+x), UP(-y). IMPORTANT NOTE: *UP*
     *        here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *posPointitive y*
     *        direction.** To reiterate, when you are exploring (filling out)
     *        from a given pixel, you must first try to fill the pixel to
     *        it's LEFT, then the one DOWN from it, then to the RIGHT and
     *        finally UP. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        Because we are doing a multi-point flood fill you will repeat the flood
     *        fill from several locations stored in the `centers` variable of FillerConfig
     *        struct. Initial fill points should be visited in the order they stored in the
     *        `centers` vector. If a pixel lies in the fill region of multiple start points,
     *        it should only be updated by the first fill. To repeat, pixels should only
     *        be modified the first time they are visited by the filler algorithm.
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and
     *        it will be the one we test against.
     *
     */

    OrderingStructure<point> osPoint;
//    OrderingStructure<colorPicker *> osPicker;

    animation animation;
    int numFilled = 0;

    PNG img = config.img;
    animation.addFrame(img);
    int w = config.img.width();
    int h = config.img.height();

    int x_neighbors[4] = {-1, 0, 1, 0};
    int y_neighbors[4] = {0, 1, 0, -1};
    bool unprocessed[w][h];
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            unprocessed[i][j] = true;
        }
    }

    for (int i = 0; i < config.centers.size(); i++) {
        point p = point(config.centers[i]);
        colorPicker *picker = config.pickers[i];

        osPoint.add(p);
//        osPicker.add(picker);

        *img.getPixel(p.x, p.y) = picker->operator()(p);
        unprocessed[p.x][p.y] = false;
        numFilled = numFilled + 1;

        if (numFilled % config.frameFreq == 0) {
            animation.addFrame(img);
        }

//        picker = osPicker.remove();

        while (!osPoint.isEmpty()) {
            point currPoint = osPoint.remove();
//        colorPicker *picker = osPicker.remove();

            for (int i = 0; i < 4; i++) {
                int x = currPoint.x + x_neighbors[i];
                int y = currPoint.y + y_neighbors[i];
                if ((0 <= x && x < w) && (0 <= y && y < h) && unprocessed[x][y] &&
                    currPoint.c.color.dist(*img.getPixel(x, y)) <= config.tolerance) {
                    point newPoint = point(x, y, currPoint.c);
                    osPoint.add(newPoint);
//                osPicker.add(picker);
                    *img.getPixel(x, y) = picker->operator()(newPoint);
                    unprocessed[x][y] = false;
                    numFilled = numFilled + 1;
                    if (numFilled % config.frameFreq == 0) {
                        animation.addFrame(img);
                    }
                }
            }
        }
    }

    if (numFilled % config.frameFreq != 0) {
        animation.addFrame(img);
    }

    return animation;
}
