from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder
import sys
import threading
import time
from uuid import uuid4
import json

endpoint = "ascvnk0a4teli-ats.iot.eu-west-1.amazonaws.com"
certs = "/home/pi/certs"

event_loop_group = io.EventLoopGroup(1)
host_resolver = io.DefaultHostResolver(event_loop_group)
client_bootstrap = io.ClientBootstrap(event_loop_group, host_resolver)


def on_connection_interrupted(connection, error, **kwargs):
    print("Connection interrupted. error: {}".format(error))


# Callback when an interrupted connection is re-established.
def on_connection_resumed(connection, return_code, session_present, **kwargs):
    print("Connection resumed. return_code: {} session_present: {}".format(return_code, session_present))
    if return_code == mqtt.ConnectReturnCode.ACCEPTED and not session_present:
        print("Session did not persist. Resubscribing to existing topics...")
        resubscribe_future, _ = connection.resubscribe_existing_topics()
        # Cannot synchronously wait for resubscribe result because we're on the connection's event-loop thread,
        # evaluate result with a callback instead.
        resubscribe_future.add_done_callback(on_resubscribe_complete)


def on_resubscribe_complete(resubscribe_future):
  resubscribe_results = resubscribe_future.result()
  print("Resubscribe results: {}".format(resubscribe_results))
  for topic, qos in resubscribe_results['topics']:
      if qos is None:
          sys.exit("Server rejected resubscribe to topic: {}".format(topic))


def send_message(message, client_id, topic):
    mqtt_connection = mqtt_connection_builder.mtls_from_path(
        endpoint=endpoint,
        cert_filepath=f"{certs}/device.pem.crt",
        pri_key_filepath=f"{certs}/private.pem.key",
        client_bootstrap=client_bootstrap,
        ca_filepath=f"{certs}/Amazon-root-CA-1.pem",
        on_connection_interrupted=on_connection_interrupted,
        on_connection_resumed=on_connection_resumed,
        client_id=client_id,
        clean_session=False,
        keep_alive_secs=30)

    message_json = json.dumps(message)
    connect_future = mqtt_connection.connect()

    # Future.result() waits until a result is available
    connect_future.result()
    print("Connected!")
    mqtt_connection.publish(
        topic=topic,
        payload=message_json,
        qos=mqtt.QoS.AT_LEAST_ONCE)

    disconnect_future = mqtt_connection.disconnect()
    disconnect_future.result()
