class Cmd::Debug
  def self.print(message)
    method_name = caller(1,1)[0][/`.*'/].tr("`'","")
    class_name  = self.name
    title = "#{class_name}.#{method_name}".ljust(20).truncate(20, omission: '')
    puts "  #{title} > #{message}"
  end
end

