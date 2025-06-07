import Cubot
import SwiftUI

struct FaceletView: View {
    let color: Cubot.Color?

    @State private var isHovering = false

    var body: some View {
        Group {
            if let color = color {
                Rectangle()
                    .foregroundColor(color.asSwiftUIColor)
            } else {
                Rectangle()
                    .foregroundColor(.gray)
            }
        }
        .onHover { isHovering in self.isHovering = isHovering }
        .border(isHovering ? .white : .clear)
    }
}
