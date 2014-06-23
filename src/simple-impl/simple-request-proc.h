/**
 * @file simple-request-proc.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 12, 2014
 * 
 * @copyright Copyright (c) Jun 12, 2014                      SOROUSH HAERI
 *            All Rights Reserved 
 *
 *            Permission to use, copy, modify, and distribute this software and its
 *            documentation for any purpose and without fee is hereby granted, provided
 *            that the above copyright notice appear in all copies and that both that
 *            copyright notice and this permission notice appear in supporting
 *            documentation, and that the name of the author not be used in advertising or
 *            publicity pertaining to distribution of the software without specific,
 *            written prior permission.
 *
 *            THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *            ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *            AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *            DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *            AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *            OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef SIMPLE_REQUEST_PROC_H_
#define SIMPLE_REQUEST_PROC_H_

#include "core/request-processor.h"

namespace vne
{

/*
 *
 */
class SimpleRequestProcessor: public RequestProcessor<
		Request<Node<int>, Link<int>>> {
public:
	SimpleRequestProcessor();
	~SimpleRequestProcessor();
	void delta_int();
	void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb);
	void delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb);
	void output_func(adevs::Bag<ADEVS_IO_TYPE>& yb);
	double ta();
	void gc_output(adevs::Bag<ADEVS_IO_TYPE>& g);
};

}
/* namespace vne */

#endif /* SIMPLE_REQUEST_PROC_H_ */
