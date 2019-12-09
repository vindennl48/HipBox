class PortsController < ApplicationController
  before_action :authenticate_user!
  before_action :is_admin
  before_action :set_port, only: [:edit, :update, :destroy]

  def index
    @in_ports        = Port.in_ports
    @out_ports       = Port.out_ports
    @in_port_groups  = PortGroup.in_port_groups
    @out_port_groups = PortGroup.out_port_groups
  end

  def new
    @port = Port.find_or_create_by(name: "new port", io: params['io'], path: "", pan: 0)
    redirect_to ports_path
  end

  def edit
  end

  def update
    respond_to do |format|
      if @port.update(port_params)
        format.html { redirect_to ports_path, notice: 'port was successfully updated.' }
        format.json { render :show, status: :ok, location: @port }
      else
        format.html { render :edit }
        format.json { render json: @port.errors, status: :unprocessable_entity }
      end
    end
  end

  def destroy
    @port.destroy
    respond_to do |format|
      format.html { redirect_to ports_path, notice: 'Port was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private

  def set_port
    @port = Port.find(params[:id])
  end

  def port_params
    params.fetch(:port, {}).permit(:id, :name, :io, :path, :pan, :port_group_id)
  end

  def is_admin
    if current_user != User.find_by(name: "Mitch")
      redirect_to channels_path
    end
  end

end
