/**
 * @file network-request.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 17, 2014
 * 
 * @copyright Copyright (c) Jun 17, 2014                      SOROUSH HAERI
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
#ifndef NETWORK_REQUEST_H_
#define NETWORK_REQUEST_H_

namespace vne
{

template<typename ...> class NetworkRequest;

template<typename ... NODERES, typename ... LINKRES>
class NetworkRequest<Node<NODERES...>, Link<LINKRES...>>
{
public:
	NetworkRequest(std::unique_ptr<Network<Node<NODERES...>, Link<LINKRES...>>> _net, std::unique_ptr<Request<Node<NODERES...>, Link<LINKRES...>>> _req);
	virtual ~NetworkRequest();
protected:
	std::unique_ptr<Network<Node<NODERES...>, Link<LINKRES...>>> net;
	std::unique_ptr<Request<Node<NODERES...>, Link<LINKRES...>>> req;
};
template<typename ... NODERES, typename ... LINKRES>
NetworkRequest<Node<NODERES...>, Link<LINKRES...>>::NetworkRequest(
		std::unique_ptr<Network<Node<NODERES...>, Link<LINKRES...>>> _net, std::unique_ptr<Request<Node<NODERES...>, Link<LINKRES...>>> _req)
		: net (std::move(_net)),
		  req (std::move(_req))
{
}
}
	/* namespace vne */

#endif /* NETWORK_REQUEST_H_ */
