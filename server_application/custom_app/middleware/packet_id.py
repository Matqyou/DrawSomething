import json
from django.http import JsonResponse

class EchoPacketIDMiddleware:
    def __init__(self, get_response):
        self.get_response = get_response

    def __call__(self, request):
        # Read packet ID from headers
        packet_id = request.headers.get("X-Packet-Id")

        request.packet_id = packet_id

        response = self.get_response(request)

        # Modify JSON body if response is JsonResponse and packet_id is valid
        if packet_id is not None and isinstance(response, JsonResponse):
            try:
                payload = json.loads(response.content)
                payload["packet_id"] = int(packet_id)
                response.content = json.dumps(payload)
                response["Content-Length"] = str(len(response.content))
            except (ValueError, json.JSONDecodeError):
                pass  # Ignore if packet_id is invalid or content isn't JSON

        return response
