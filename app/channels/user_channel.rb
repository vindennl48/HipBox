class UserChannel < ApplicationCable::Channel
  def subscribed
    @user = User.find(params[:room]["id"])
    stream_for @user
  end

  def unsubscribed
    # Any cleanup needed when channel is unsubscribed
  end

  # send data to client/browser
  def askForData
    variables = Variable.where(user_id: @user.id)
    UserChannel.broadcast_to(@user, {records: variables})
  end

  # receive data from client/browser
  def sendData(data)
    record = data["record"]
    Variable.update_record(record)
  end

end
