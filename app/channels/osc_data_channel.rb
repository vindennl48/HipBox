class OscDataChannel < ApplicationCable::Channel
  def subscribed
    @variable = Variable.find_by(name: params[:room])
    stream_for @variable
  end

  def unsubscribed
    # Any cleanup needed when channel is unsubscribed
  end

  # send data to client/browser
  def askForData
    @variable = Variable.find(@variable.id)
    OscDataChannel.broadcast_to(@variable, {record: @variable})
  end

  # receive data from client/browser
  def sendData(data)
    record = data["record"]
    Variable.process_incoming(record:record)
  end

end
