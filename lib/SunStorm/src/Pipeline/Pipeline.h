/* 
 * File:   Pipeline.h
 * Author: majo32
 *
 * Created on Utorok, 2016, marca 29, 18:24
 */
#pragma once
#ifndef SUNSTORM_PIPELINE_H
#define	SUNSTORM_PIPELINE_H
namespace SunStorm {

    class Pipeline {
    public:

        Pipeline() {
        }
        Pipeline(const Pipeline& orig) = delete;

        virtual ~Pipeline() {
        }
    private:

    };
};
#endif	/* PIPELINE_H */

