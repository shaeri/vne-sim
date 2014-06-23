/**
 * @file request-processor.h
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
#ifndef REQUEST_PROCESSOR_H_
#define REQUEST_PROCESSOR_H_

#include "request.h"
#include "adevs.h"

namespace vne
{
template<typename...> class RequestProcessor;

template<typename ... NODERES, typename ... LINKRES>
class RequestProcessor<Request<Node<NODERES...>, Link<LINKRES...>>>
		: public adevs::Atomic<adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>* >>
{
public:
	RequestProcessor();
	virtual ~RequestProcessor();
	virtual void delta_int() = 0;
	virtual void delta_ext(double e, const adevs::Bag<adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>*>>& xb) = 0;
	virtual void delta_conf(const adevs::Bag<adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>*>>& xb) = 0;
	virtual void output_func(adevs::Bag<adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>*>>& yb) = 0;
	virtual double ta() = 0;
	virtual void gc_output(adevs::Bag<adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>*>>& g) = 0;

	typedef adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>* > ADEVS_IO_TYPE;
};

template<typename ... NODERES, typename ... LINKRES>
RequestProcessor<Request<Node<NODERES...>, Link<LINKRES...>>>::RequestProcessor()
 :adevs::Atomic<adevs::PortValue<Request<Node<NODERES...>, Link<LINKRES...>>* >> ()
{
}
template<typename ... NODERES, typename ... LINKRES>
RequestProcessor<Request<Node<NODERES...>, Link<LINKRES...>>>::~RequestProcessor()
{
}
}

#endif /* REQUEST_PROCESSOR_H_ */
